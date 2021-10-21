packages <- c("rmarkdown")

install.packages(setdiff(packages, rownames(installed.packages())))  