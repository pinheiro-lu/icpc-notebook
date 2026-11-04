use std::fs;
use std::io::{Read, Write as ioWrite};
use std::process::Stdio;
use std::sync::mpsc;

fn md5sum(s: &str) -> String {
    // Because lines are 1-indexed
    let cmd = std::process::Command::new("sh")
        .arg("-c")
        .arg("cpp -dD -P -fpreprocessed | tr -d '[:space:]' | md5sum | cut -c-6")
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("Failed to start command");

    let mut stdin = cmd.stdin.as_ref().expect("Failed to open stdin");
    stdin
        .write_all(s.as_bytes())
        .expect("Failed to write to stdin");
    String::from_utf8(
        cmd.wait_with_output()
            .unwrap()
            .stdout
            .into_iter()
            .take(3)
            .collect(),
    )
    .unwrap()
}

fn ignore_line(line: &str) -> bool {
    line.starts_with("#warning")
}

/// Returns the hashes list of a file
fn hashes(lines: &[String], path: &str) -> Vec<String> {
    let mut scopes: Vec<usize> = Vec::new();

    let mut hashes = Vec::new();

    for (index, line) in lines.iter().enumerate() {
        if ignore_line(line) {
            hashes.push("---".to_string());
            continue;
        }
        let mut scope = index;
        for c in line.chars() {
            if c == '{' {
                scopes.push(index);
            } else if c == '}' {
                scope = *scopes.last().unwrap_or_else(|| {
                    panic!(
                        "Parse error with scopes in file {path} in line {}",
                        index + 1
                    )
                });
                scopes.pop();
            }
        }
        let scope_code = lines[scope..=index]
            .iter()
            .map(String::as_str)
            .filter(|&s| !ignore_line(s))
            .collect::<Vec<_>>()
            .join("\n");
        hashes.push(md5sum(&scope_code));
    }
    hashes
}

/// For each section, do something like this
/// == Section
/// === Code
/// #cpp(`int main() {
/// }`, hashes: ("foo", "bar"))
fn codes(codes_dir_path: &str) -> Result<String, std::fmt::Error> {
    let mut sections = fs::read_dir(codes_dir_path)
        .expect("Path `code/ doesn't exists!")
        .map(|entry| entry.expect("Failed to get entry").path())
        .collect::<Vec<_>>();
    sections.sort_by_key(|p| p.file_name().unwrap().to_string_lossy().to_lowercase());

    let mut results = Vec::new();

    let (send, recv) = mpsc::channel::<String>();

    for section in sections {
        let send = send.clone();
        std::thread::spawn(move || {
            let section_file_name = section.file_name();
            let section_file_name_str = section_file_name.unwrap().to_string_lossy();

            let (code_send, code_recv) = mpsc::channel::<String>();
            for code in fs::read_dir(section.as_path())
                .unwrap_or_else(|_| panic!("{section_file_name_str} is not a directory!"))
                .map(|entry| entry.expect("Failed to get entry").path())
            {
                let code_send = code_send.clone();
                std::thread::spawn(move || {
                    let file_name: String = code.file_name().unwrap().to_string_lossy().to_string();
                    let mut file =
                        fs::File::open(&code).unwrap_or_else(|_| panic!("{code:?} is not a file!"));

                    let mut content = String::new();
                    file.read_to_string(&mut content).unwrap();

                    if content.find('`').is_some() {
                        panic!("backticks (\"`\") are not supported");
                    }

                    let lines: Vec<String> = content.lines().map(|s| s.to_owned()).collect();

                    code_send
                        .send(format!(
                            "=== {name}\n#cpp(`{all_lines}`, hashes: ({all_hashes}))\n",
                            name = file_name.trim_end_matches(".cpp"),
                            all_lines = &lines.join("\n"),
                            all_hashes = &hashes(&lines, &code.as_path().to_string_lossy())
                                .into_iter()
                                .map(|s| format!("\"{s}\""))
                                .collect::<Vec<_>>()
                                .join(",")
                        ))
                        .unwrap();
                });
            }
            drop(code_send);
            let mut codes = Vec::new();
            while let Ok(code) = code_recv.recv() {
                codes.push(code);
            }
            codes.sort();
            send.send(format!(
                "== {section_file_name_str}\n{codes}\n",
                codes = codes.join("")
            ))
            .unwrap();
        });
    }
    drop(send);

    while let Ok(result) = recv.recv() {
        results.push(result);
    }

    results.sort();

    Ok(results.join(""))
}

fn shift_even_pages(pdf_path: &str) {
    const ORIG: &[u8] = b"/MediaBox[0 0 841.8898 595.2756]";
    const EVEN: &[u8] = b"/MediaBox[0 -19.84 841.9 575.43]";
    assert_eq!(ORIG.len(), EVEN.len());

    let mut data = fs::read(pdf_path).expect("Failed to read PDF");
    let pages_tag = b"/Type/Pages";
    let pages_pos = data
        .windows(pages_tag.len())
        .position(|w| w == pages_tag)
        .expect("Could not find /Type/Pages in PDF");
    let kids_tag = b"/Kids[";
    let kids_rel = data[pages_pos..]
        .windows(kids_tag.len())
        .position(|w| w == kids_tag)
        .expect("Could not find /Kids[ in /Type/Pages");
    let kids_start = pages_pos + kids_rel + kids_tag.len();
    let kids_end = kids_start
        + data[kids_start..]
            .iter()
            .position(|&b| b == b']')
            .expect("Unclosed /Kids[");
    let kids_str = String::from_utf8(data[kids_start..kids_end].to_vec()).unwrap();
    let tokens: Vec<&str> = kids_str.split_whitespace().collect();
    for (idx, chunk) in tokens.chunks_exact(3).enumerate() {
        let phys_page = idx + 1;
        if phys_page % 2 == 0 {
            let obj_header = format!("\n{} 0 obj\n", chunk[0]);
            let obj_bytes = obj_header.as_bytes();
            let obj_pos = data
                .windows(obj_bytes.len())
                .position(|w| w == obj_bytes)
                .unwrap_or_else(|| panic!("Could not find object {}", chunk[0]));
            let mb_rel = data[obj_pos..]
                .windows(ORIG.len())
                .position(|w| w == ORIG)
                .unwrap_or_else(|| panic!("Could not find /MediaBox in page {phys_page}"));
            let mb_pos = obj_pos + mb_rel;
            data[mb_pos..mb_pos + EVEN.len()].copy_from_slice(EVEN);
        }
    }
    fs::write(pdf_path, &data).expect("Failed to write shifted PDF");
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() == 3 && args[1] == "--duplex-shift" {
        shift_even_pages(&args[2]);
        return;
    }
    let codes_path = std::env::var("CODES_DIR_PATH")
        .expect("Set the path of the codes directory as the CODES_DIR_PATH environment variable");
    let codes = codes(&codes_path).unwrap();
    println!("{codes}");
}
