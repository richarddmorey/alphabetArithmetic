## The following code requires the R libraries
#   here
#   dplyr
#   readr
#   purrr
#   glue
##

here::here("data") %>%
  dir(full.names = TRUE) %>%
  purrr::map_df(readr::read_delim, delim = " ",
                col_names = c("id", "sex", "trial", "letter", "addend", "sum", 
                              "stim_id", "is_true", "is_repeat", "repeat_num", 
                              "response", "correct", "rt"),
                col_types = c(
                  correct = readr::col_integer()
                )
  ) %>%
  mutate(
    # Make the response times numeric
    rt = as.numeric(rt),
    # Make the repeat counts integers
    repeat_num = as.integer(repeat_num),
    # get rid of the prefixes
    across(where(is.character), gsub, pattern = "[a-zA-Z]", repl = ""),
    # turn everything else into integers
    across(where(is.character), as.integer),
    # create clearer factors where necessary
    id = factor(id),
    sex = factor(sex, labels = c("female","male")),
    letter_chr = factor(LETTERS[letter], ordered = TRUE),
    sum_chr = factor(LETTERS[sum], ordered = TRUE),
    equation_chr = factor(glue::glue("{letter_chr}+{addend}={sum_chr}"))
  ) -> aa
