## Contributing

Contributions are welcome! If you have ideas for improvements, bug fixes, or
new features, please open an issue or submit a pull request. For larger
changes, it’s a good idea to discuss your proposal in an issue first.

To contribute code:

1. Fork the repository and create your branch from `main`.
2. Ensure your code is well-tested and passes all existing tests.
3. Run `cmake` and the test suite to verify.
4. Open a pull request with a clear description of your changes.

Thank you for helping make qualpal better!

### Code Style

Please format C++ code using `clang-format` before
submitting a pull request. The project includes a `.clang-format` file
and uses the Mozilla style guide as a base, although
not in terms of naming conventions, where (in short)
we instead use upper CamcelCase for class names,
lower camelCase for function names, and snake_case for variables.

### Commit Guidelines

Use [Conventional Commits](https://www.conventionalcommits.org/) for all commit messages.
Example:

```
feat(cli): add `--output-delim` option
fix: correct color contrast calculation
```
