make clean && make -j4
git pull  https://github.com/RutgersCSHeteroLab/ParaFSpaper atc
git commit -am "$1"
git push origin atc
