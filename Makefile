CODES_DIR_PATH ?= "brazilian_finals/"
DUPLEX ?= 0

docker-pdf: docker-image
	docker run -v ./output:/app/output icpc-notebook-template-image:dev && cp ./output/main.pdf ./notebook.pdf && rm -f output/*
.PHONY: docker-pdf

docker-image:
	docker build . -t icpc-notebook-template-image:dev
.PHONY: docker-image

pdf: typst
ifeq ($(DUPLEX),1)
	typst compile --input duplex=true ./output/main.typ && ./output/main --duplex-shift ./output/main.pdf && cp ./output/main.pdf ./notebook.pdf
else
	typst compile ./output/main.typ && cp ./output/main.pdf ./notebook.pdf
endif
.PHONY: pdf

typst: compile
	cp pdf-generator/* ./output
	CODES_DIR_PATH=$(CODES_DIR_PATH) output/main >> ./output/main.typ
.PHONY: typst

compile: pdf-generator/main.rs
	rustc --edition=2021 -O pdf-generator/main.rs -o output/main
.PHONY: compile

clean:
	docker image rm -f icpc-notebook-template-image:dev
	rm -f notebook.pdf output/*
.PHONY: clean
