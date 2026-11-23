# Command-line

Run `qualpal [options] <values>` to generate a palette. The command prints the
selected colors as hex values, one per line by default.

```bash
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"
```

## Inputs

| Input mode | Values |
| --- | --- |
| `hex` (default) | A list of quoted hex colors, such as `"#ff0000" "#0000ff"`. |
| `colorspace` | Three ranges in hue, saturation, and lightness order. Use degrees from 0 to 360 for hue and values from 0 to 1 for saturation and lightness. |
| `palette` | A built-in palette name, such as `"ColorBrewer:Set1"`. |

## Options

| Option | Purpose |
| --- | --- |
| `-n`, `--number` | Total number of colors to return. Defaults to 8. |
| `-i`, `--input` | Input mode. |
| `--extend` | Hex colors to keep in the output palette. |
| `-b`, `--background` | Background color as a quoted hex value. |
| `--deutan`, `--protan`, `--tritan` | Severity of each color vision deficiency to simulate, from 0 to 1. |
| `-p`, `--points` | Number of candidate points to sample in colorspace mode. Defaults to 1,000. |
| `-m`, `--metric` | Color difference metric. Choose `ciede2000` (default), `din99d`, or `cie76`. |
| `--max-memory` | Maximum memory use in GB. Defaults to 1.0. |
| `--output-delim` | Output separator. Choose `newline` (default), `space`, or `comma`. |
| `--colorize` | Colorize terminal output. Choose `auto` (default), `always`, or `never`. |

## Commands

- `qualpal list-palettes` lists the available built-in palettes.
- `qualpal analyze <values>` computes a color difference matrix for a list of
  hex colors. Use `qualpal analyze -i palette <name>` to analyze a built-in palette.
- `qualpal help` prints the full help text.

See [command-line examples](../docs/examples.md#examples-cli) for more ways to
generate palettes.
