# Alphabet Arithmetic 

This repository contains data from a cognitive experiment performed in the mid-2000s. 

For experimental details, an interactive example of the task, and data column descriptions see https://richarddmorey.github.io/alphabetArithmetic/.

## *Super* quick start

1. Go to https://richarddmorey.github.io/alphabetArithmetic/.

2. At the bottom of the page, click the orange "Download tidied data" button.

3. Open the `.csv` file in your analysis program of choice.

## Quick start for R users

1. Download the repository using the green button to the top right. Choose "Download ZIP".

![Choose "Download ZIP"](https://raw.githubusercontent.com/richarddmorey/alphabetArithmetic/master/explainer/include/img/download_code.png)

2. Unzip the repository and open up the RStudio project `alphabetArithmetic.Rproj`.

3. Install the necessary packages:

```
install.packages(c("here", "dplyr", "readr", "purrr", "glue"))
```

4. Run the code in `load_data.R`. The data will be in a tibble called `aa`, ready for analysis.

## Contents:

| Item         | Description                             |
|:-------------|:----------------------------------------|
| `load_data.R`| R code to load the data
| `data`       | Contains all the data                   |
| `explainer`  | Contains the Rmarkdown code to generate the website describing the data |
| `task_code`  | Contains the original experimental code |
| `docs`       | GitHub pages folder containing compiled explainer document |




