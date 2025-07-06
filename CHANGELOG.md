# Changelog

## [0.10.0](https://github.com/jolars/qualpal/compare/v0.9.0...v0.10.0) (2025-07-06)


### ⚠ BREAKING CHANGES

* switch to public/private header setup

### Features

* add `colorDifferenceMatrix()` ([577ef46](https://github.com/jolars/qualpal/commit/577ef4605a29f0cae3461748f4f77cf36893bd92))
* add color difference metrics, and put into namespace ([577ef46](https://github.com/jolars/qualpal/commit/577ef4605a29f0cae3461748f4f77cf36893bd92))
* **cli:** add help command ([9bd0ec0](https://github.com/jolars/qualpal/commit/9bd0ec0de75ce77a5a60da0a6d0e74919d2dbaef))
* expose `Matrix` and `FixedMatrix` classes ([577ef46](https://github.com/jolars/qualpal/commit/577ef4605a29f0cae3461748f4f77cf36893bd92))
* switch to public/private header setup ([ea5047f](https://github.com/jolars/qualpal/commit/ea5047f583bbf434bba10cc7e1d96673b691da76))


### Bug Fixes

* **cli:** use `cerr` to propagate exceptions ([15ef5f9](https://github.com/jolars/qualpal/commit/15ef5f9c67046fbb3c77556ff66912b342042dd9))
* correctly setup include directory ([96b0759](https://github.com/jolars/qualpal/commit/96b07599ab9917b1b6f142c9a22a3e6d1fe35e57))
* use library umbrealla header in cli ([5f56332](https://github.com/jolars/qualpal/commit/5f56332dcf4ca45f31505a2b0f61315894428162))

## [0.9.0](https://github.com/jolars/qualpal/compare/v0.8.0...v0.9.0) (2025-04-12)


### ⚠ BREAKING CHANGES

* require utf8 in input
* remove bindings (r)
* remove n_threads from API
* revert removal of bindings

### Features

* add convert_colors and organize output ([1047f9a](https://github.com/jolars/qualpal/commit/1047f9ab62d3bd14fa901f062adb7a0f75583c60))
* add string method to use color palette as base ([a1b6e2e](https://github.com/jolars/qualpal/commit/a1b6e2e78d128869886d3dc9a21978dbdc617432))
* only return colors when called in cli ([2bbea10](https://github.com/jolars/qualpal/commit/2bbea10416dc1685c8e4cf5e0cc0ee0dce17e800))
* remove bindings (r) ([571a0b4](https://github.com/jolars/qualpal/commit/571a0b40fa9c546a66b63cfbc7ab3e499859111e))
* remove n_threads from API ([dd44576](https://github.com/jolars/qualpal/commit/dd4457620d1350822e4464e60f39ae5a4e45b0e8))
* require utf8 in input ([d90f1c2](https://github.com/jolars/qualpal/commit/d90f1c201ffe57371e3bbb687f39215d12106533))
* revert removal of bindings ([f4e4905](https://github.com/jolars/qualpal/commit/f4e490544fe978f6325fa0ae6ac8b55ce695ec59))
* **r:** export convert_colors() function ([07d5d11](https://github.com/jolars/qualpal/commit/07d5d11d330d45f8b2d83c4112691f5564c772da))


### Bug Fixes

* initialize counter and increment after loop ([19e9797](https://github.com/jolars/qualpal/commit/19e9797f84556630155b5054111584aa5d6273a4))
* **r:** fix formatting of documentation, dropping brackets ([1090a04](https://github.com/jolars/qualpal/commit/1090a04d073eada01f3c35b07bfc678f1ee9d3f5))


### Reverts

* "docs: remove stuff about bindings from readme" ([8b31493](https://github.com/jolars/qualpal/commit/8b31493cc1a7ff86b037e7628c76c89700ed3559))

## [0.8.0](https://github.com/jolars/qualpal/compare/v0.7.3...v0.8.0) (2024-06-10)


### ⚠ BREAKING CHANGES

* switch from grid to Halton sequence

### Features

* switch from grid to Halton sequence ([f1eefc3](https://github.com/jolars/qualpal/commit/f1eefc3dfe7781ec771a97fd354629128a877f8f))

## [0.7.3](https://github.com/jolars/qualpal/compare/v0.7.2...v0.7.3) (2024-06-10)


### Bug Fixes

* fix hue adjustment ([4e9eec1](https://github.com/jolars/qualpal/commit/4e9eec1f1242ab8c6f1c4e91af2b491e451e517a))

## [0.7.2](https://github.com/jolars/qualpal/compare/v0.7.1...v0.7.2) (2024-06-10)


### Bug Fixes

* use correct custom mod() implementation ([c2fd458](https://github.com/jolars/qualpal/commit/c2fd458bfa68c7120855d4f18f2a52edad444ef8))

## [0.7.1](https://github.com/jolars/qualpal/compare/v0.7.0...v0.7.1) (2024-06-10)


### Bug Fixes

* don't return nan values in rgb to hsl conversion ([284af67](https://github.com/jolars/qualpal/commit/284af6740645420daa4f97b68c75848fa5aca35e))

## [0.7.0](https://github.com/jolars/qualpal/compare/v0.6.0...v0.7.0) (2024-06-07)


### Features

* allow negative hues to let user pick any hue range ([f6d5f73](https://github.com/jolars/qualpal/commit/f6d5f731e2632e47ba7c0593680bcf471ffc52f5))

## [0.6.0](https://github.com/jolars/qualpal/compare/v0.5.0...v0.6.0) (2024-06-07)


### Features

* add conversions between more colors ([92ed947](https://github.com/jolars/qualpal/commit/92ed9479caad315500906b222d4f6a7f87f8b7b5))


### Bug Fixes

* use correct type in comparison ([1a2c7c4](https://github.com/jolars/qualpal/commit/1a2c7c401c0f8a7151900baf1febd3caa3df0515))

## [0.5.0](https://github.com/jolars/qualpal/compare/qualpal-v0.4.5...qualpal-v0.5.0) (2024-06-04)


### ⚠ BREAKING CHANGES

* remove r bindings from repo

### Features

* make R binding work properly ([17f369c](https://github.com/jolars/qualpal/commit/17f369c0737c5f99769c3eac8b840d33e4b8cfad))
* merge individual color files into a single file ([1e2e74c](https://github.com/jolars/qualpal/commit/1e2e74c9554546e5e5b24ba6bb4405f226216359))
* remove r bindings from repo ([db09c17](https://github.com/jolars/qualpal/commit/db09c17dfbc9bc96d8f08adca270d6179bdb5eef))
* rename hsl_grid to color_grid ([9a32722](https://github.com/jolars/qualpal/commit/9a327220f19ea12bea146fab9f1eb18c8164031c))
* setup infrastructure for the package ([14e2800](https://github.com/jolars/qualpal/commit/14e280015cb05420b7093b3ba487e32958ccebfd))
* **tests:** add more conversion tests ([50e5a9d](https://github.com/jolars/qualpal/commit/50e5a9d81c35947cb2dc69abb2b06cda85fb14d8))


### Bug Fixes

* add missing build step in makefile ([ad6c84d](https://github.com/jolars/qualpal/commit/ad6c84d461a566897f946f276b4516307bc15cdf))
* add missing definition for qualpal() ([3ccf51b](https://github.com/jolars/qualpal/commit/3ccf51bddbdf58fb98341f9d4c37027034f9752a))
* **cpp:** replace fmod with custom mod function ([0fe84d4](https://github.com/jolars/qualpal/commit/0fe84d4ef531e3ed2053d15cf2fe3033f1907ed8))
* make library complete ([a4db314](https://github.com/jolars/qualpal/commit/a4db314ae71c353abafc588783ca1dca93f74638))
* use correct name for function ([7abefee](https://github.com/jolars/qualpal/commit/7abefee6685fb863d54233c58c5d7cd2c2d46807))
* use int instead of double in hex to rgb color conversion ([50fe9a0](https://github.com/jolars/qualpal/commit/50fe9a02b42a0222f7947203a0a993b6bade3135))

## 0.5.0

### Features

- push an initial release of the c++ version of the package
