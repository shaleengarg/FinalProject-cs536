all: main

main: main.tex
	#rm -rf main.pdf
	pdflatex main.tex
	bibtex main
	pdflatex main.tex
	pdflatex main.tex

clean:
	rm -rf *.log main.pdf *.aux *.toc *.dvi *.bbl *.blg *.ps *.out

errors:
	#Missing citations
	find . -name 'main.pdf' -exec sh -c 'pdftotext "{}" - | grep --with-filename --label="{}" --color "?"' \;
	find . -name 'main.pdf' -exec sh -c 'pdftotext "{}" - | grep --with-filename --label="{}" --color "[]"' \;
	find . -name 'main.pdf' -exec sh -c 'pdftotext "{}" - | grep --with-filename --label="{}" --color "%ada"' \;
	#cat abstract.tex intro.tex background.tex analysis.tex > paper.tex
	#cat paper.tex | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq -c | sort -bnr | grep $2 | more
