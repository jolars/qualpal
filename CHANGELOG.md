# Changelog

## [2.4.0](https://github.com/jolars/qualpal/compare/v2.3.0...v2.4.0) (2025-07-28)

### Features

* use `std::size_t` in place of `int` where appropriate ([d22ed8b](https://github.com/jolars/qualpal/commit/d22ed8be4dfee0552e5427217be7bba4d0deb203))

### Bug Fixes

* correct some diagnostic warnings ([fe3254c](https://github.com/jolars/qualpal/commit/fe3254c8f8fe4dc1e9d42caf35e9b419b342b193))

### Performance Improvements

* avoid copy in `farthestPoints()` ([3c853f6](https://github.com/jolars/qualpal/commit/3c853f6ffc6484df76370080a72bb13fa5514648))

## [2.3.0](https://github.com/jolars/qualpal/compare/v2.2.0...v2.3.0) (2025-07-21)

### Features

* **colors:** add default constructors for all colors ([9aa605b](https://github.com/jolars/qualpal/commit/9aa605b3c1a96e353da41136251c249de8637f39))
* **colors:** add equality and inequality operators ([8f8988c](https://github.com/jolars/qualpal/commit/8f8988ce18f6b7104d7fbe6bc3790d04981df771))
* **colors:** add LCHab color space ([572cb11](https://github.com/jolars/qualpal/commit/572cb114c34ef4364274d28f50294ddcd807a1c0))
* **core:** add `extend()` to `Qualpal` ([3f31208](https://github.com/jolars/qualpal/commit/3f31208feb00f095dc103de9558d966062b12d98))
* support using LCHab as colorspace input ([78ed8a8](https://github.com/jolars/qualpal/commit/78ed8a8112a7dcf10aef6e855ef74fdc78589f3a))

### Bug Fixes

* clamp values in `scaleToInterval()` ([29c837f](https://github.com/jolars/qualpal/commit/29c837f9d177187a7840649b5ab5d3761220c249))
* size `r_c` to be `n_candidates` long ([5c7d636](https://github.com/jolars/qualpal/commit/5c7d6367c877abf860906a10097e8efa5ab7bd02))
* use template instantiation to correctly grid in LCHab ([f3aab33](https://github.com/jolars/qualpal/commit/f3aab33846007318f56a5cfb0849fbd93f33dbe2))

## [2.2.0](https://github.com/jolars/qualpal/compare/v2.1.0...v2.2.0) (2025-07-17)

### Features

* add new function `analyzePalette` ([15c9059](https://github.com/jolars/qualpal/commit/15c90590988c8a0a7c2ff670bf0824e415cd3fd3))
* **cli:** add `--colorize` option to colorize output colors ([bd93424](https://github.com/jolars/qualpal/commit/bd934247c0a90b6e140b4b6d86ebf854edb13292))
* **cli:** add `--output-delim` option ([7177008](https://github.com/jolars/qualpal/commit/71770085376349b3b6b640cbddd5fa233a807192))

### Bug Fixes

* **cli:** move `use_color` down ([b3627bd](https://github.com/jolars/qualpal/commit/b3627bdb36b4df4201580cfff25820495b8baef2))
* **cli:** move undefine for RGB ([178d504](https://github.com/jolars/qualpal/commit/178d504c30cd8482bc3a381b92f7654502be5e4c))
* **cli:** undefine RGB on windows machines ([cec457c](https://github.com/jolars/qualpal/commit/cec457cb8e7c5c32c4326b539c64ae5ddc084b7a))

## [2.1.0](https://github.com/jolars/qualpal/compare/v2.0.0...v2.1.0) (2025-07-16)

### Features

* add additional palettes ([cfd145a](https://github.com/jolars/qualpal/commit/cfd145ae467e086ce6c3f0fa8ad6f1116383eeeb))
* **cli:** add subcommand to list available palettes ([70c5741](https://github.com/jolars/qualpal/commit/70c574100e651f0dc13cc524f70c3ea45a2ee21f))

## [2.0.0](https://github.com/jolars/qualpal/compare/v1.0.0...v2.0.0) (2025-07-16)

### ⚠ BREAKING CHANGES

* change main interface to builder pattern
* use XYZ colors in color diff matrix
* allow choosing metric for `qualpal()`
* hide internal functions behind detail namespace

### Features

* add reference white to XYZ-Din99d conversion ([b95ea7d](https://github.com/jolars/qualpal/commit/b95ea7d3857f289e8958c4580a91b4debbb6a345))
* allow choosing metric for `qualpal()` ([0e9c6e6](https://github.com/jolars/qualpal/commit/0e9c6e6d4ca8a39655d72ad7e3558f55964f8a13))
* change main interface to builder pattern ([80c7251](https://github.com/jolars/qualpal/commit/80c72517125f4764f5c9e261a6ba79eebbaa1930))
* **cli:** add `--background`, `-b` to consider bg color ([c7e498e](https://github.com/jolars/qualpal/commit/c7e498e02dcf38367425a3f14122cafccfef89f2))
* **cli:** add `-m,--metric` to choose metric type ([d2f19a7](https://github.com/jolars/qualpal/commit/d2f19a7636d30e7a03b7e9f49d098af9e9afb65b))
* **cli:** add `-p`, `--points` arguments ([1fa8d3f](https://github.com/jolars/qualpal/commit/1fa8d3f58c8a92ef67a4c9cb25e1bd883bf13332))
* **cli:** make `max_memory` a base argument ([5588f76](https://github.com/jolars/qualpal/commit/5588f7604e5e0a865566ff79e050702605a75492))
* **matrix:** add empty constutor for `Matrix` ([7ca7750](https://github.com/jolars/qualpal/commit/7ca775059104587af834dfe73258e8052819f2ba))
* **metrics:** add CIEDE2000 metric ([4924bed](https://github.com/jolars/qualpal/commit/4924bedce190b220af20e6fb6bdf501801b426ff))
* use XYZ colors in color diff matrix ([a8c53f5](https://github.com/jolars/qualpal/commit/a8c53f5e9ddc86d2ad4dbb460e9eb97d9d4b4f85))

### Bug Fixes

* adjust white point in xyz to din99d conversion ([572ef72](https://github.com/jolars/qualpal/commit/572ef728e29fc40eec60701fb30b88ed4fed7ce0))
* correctly consider CIE76 ([c172417](https://github.com/jolars/qualpal/commit/c1724170841a96757a339e5e51e733b684445cb5))
* create color vectors to color difference matrix ([71dd8ad](https://github.com/jolars/qualpal/commit/71dd8adc19042cf432de9f8e1a1b8e450f8ce8ac))
* make `max_memory` double ([3cc48da](https://github.com/jolars/qualpal/commit/3cc48da293bf6c93d7233a4cf253ce41d068de16))
* use `din99d_colors` in din99d distance computation ([ee101ca](https://github.com/jolars/qualpal/commit/ee101cad246032bab2c66a113c181747441343b7))

### Performance Improvements

* precompute background color when computing distances ([3c50dc6](https://github.com/jolars/qualpal/commit/3c50dc6d7fd571595e5b5040de40d38cfe5afeb9))

### Code Refactoring

* hide internal functions behind detail namespace ([87f7837](https://github.com/jolars/qualpal/commit/87f7837d7561984f1b1ec24cfa78dc0d037b7a86))

## [1.0.0](https://github.com/jolars/qualpal/compare/v0.12.0...v1.0.0) (2025-07-14)

### ⚠ BREAKING CHANGES

* change from "deuter" to "deutan"

### Features

* also simulate CVD for background color ([954dbfe](https://github.com/jolars/qualpal/commit/954dbfe9f804a2efd70244872c0eac36b06b92e8))
* change from "deuter" to "deutan" ([16d7c64](https://github.com/jolars/qualpal/commit/16d7c64cf9e676bb894f58ba6852a1081fa94377))

### Bug Fixes

* clamp return values from CVD simulation ([fc04fd1](https://github.com/jolars/qualpal/commit/fc04fd1ae82811b85e2b795c206210165400814a))
* correctly interpolate matrices ([2b3ab23](https://github.com/jolars/qualpal/commit/2b3ab236b1c2aaaf50f088a489fb28e9b118f694))

### Performance Improvements

* pass colors by const ref ([09e1965](https://github.com/jolars/qualpal/commit/09e196533ef0dbc5f6cbfe84a45794f9b1605228))
* set execution policy to `par` for `transform` ([71f745e](https://github.com/jolars/qualpal/commit/71f745eb06a638665c65c39e09a741fc0c681365))

### Reverts

* "feat!: change `deutan` to `deuter` for CVD options" ([45510c5](https://github.com/jolars/qualpal/commit/45510c5e4759d00c787d9d5daa65b16e4ed550e3))

## [0.12.0](https://github.com/jolars/qualpal/compare/v0.11.0...v0.12.0) (2025-07-12)


### ⚠ BREAKING CHANGES

* change `deutan` to `deuter` for CVD options

### Features

* change `deutan` to `deuter` for CVD options ([d1e6ab1](https://github.com/jolars/qualpal/commit/d1e6ab18e1bbcf14f901573b3f66bd0833c00141))
* **cli:** add `deuter`, `tritan`, `protan` for CVD simul ([4568750](https://github.com/jolars/qualpal/commit/4568750da10b2a0ea720424e22fc6311a97a67c1))
* **cli:** add subcommand to analyze color palette ([ed818e3](https://github.com/jolars/qualpal/commit/ed818e3f42a79598890e97950708a794da52d3ca))
* make FixedMatrix constructors and members `constexpr` ([42ac95d](https://github.com/jolars/qualpal/commit/42ac95d860251b0010692973be4b6eacd29ef2c4))
* set a default limit on the color diff matrix size ([9bbd5a1](https://github.com/jolars/qualpal/commit/9bbd5a1d8072bcccae22dabb43597c2b25e927f5))


### Bug Fixes

* correctly handle negatives hues in hsl to rgb conversion ([51c5d12](https://github.com/jolars/qualpal/commit/51c5d12e88f08bd80f5a7e30c08c13a486e68326))


### Performance Improvements

* parallelize with openmp ([4a3c10b](https://github.com/jolars/qualpal/commit/4a3c10b3dc43a7f1d2db732d99984038950ed670))
* use `constexpr` cvd matrices in `simulateCvd` ([3bf53a1](https://github.com/jolars/qualpal/commit/3bf53a179f30a784d38019267d4f018d441dae16))
* use `string_view` in `simulateCvd` ([e3b3ad3](https://github.com/jolars/qualpal/commit/e3b3ad3254bd40c278b6b35f9431c54c4d6d32bb))

## [0.11.0](https://github.com/jolars/qualpal/compare/v0.10.0...v0.11.0) (2025-07-09)


### ⚠ BREAKING CHANGES

* put submodules in `qualpal/` not `qualpal_bits/`
* add extension to main umbrella header
* wrap color classes in `colors` namespace

### Features

* add `bg` option to consider background color ([ea34955](https://github.com/jolars/qualpal/commit/ea349550b9752e57ccc5030910685913ed7591e5))
* extend metrics to handle disparate color types ([6b9faa2](https://github.com/jolars/qualpal/commit/6b9faa29527e0c71e7f05f1ad72a885cc3b15f11))
* wrap color classes in `colors` namespace ([d5bf8ce](https://github.com/jolars/qualpal/commit/d5bf8ce46beb0805c02e010710df0a7ee7c60f99))


### Bug Fixes

* correct set update logic in `farthestPoints()` ([98d97b9](https://github.com/jolars/qualpal/commit/98d97b959aef2d240a4cbaad87d46111b3708a85))


### Performance Improvements

* improve performance of background distance computations ([09cfcbe](https://github.com/jolars/qualpal/commit/09cfcbe67071d9fda8eb12ce88918667a30840fc))


### Code Refactoring

* add extension to main umbrella header ([c89cb71](https://github.com/jolars/qualpal/commit/c89cb71bea5967b849c637bf95909899cc4d5058))
* put submodules in `qualpal/` not `qualpal_bits/` ([c89cb71](https://github.com/jolars/qualpal/commit/c89cb71bea5967b849c637bf95909899cc4d5058))

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
