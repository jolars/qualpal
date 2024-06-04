library(qualpalr)

test_that("colors are within their respective color spaces", {
  fit <- qualpal(n = 3)
  expect_true(all(fit$RGB >= 0 & fit$RGB <= 1))
  expect_true(all(fit$HSL[, 1] >= 0 & fit$HSL[, 1] <= 360))
  expect_true(all(fit$HSL[, 2] >= 0 & fit$HSL[, 2] <= 1))
  expect_true(all(fit$HSL[, 2] >= 0 & fit$HSL[, 2] <= 1))
})
