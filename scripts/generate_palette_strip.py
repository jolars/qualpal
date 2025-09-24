import argparse
import sys
from pathlib import Path

import matplotlib.pyplot as plt


def parse_colors(output: str) -> list[str]:
    output = output.strip()
    if "," in output:
        return [c.strip() for c in output.split(",") if c.strip()]
    elif " " in output:
        return [c.strip() for c in output.split(" ") if c.strip()]
    else:
        return [c.strip() for c in output.splitlines() if c.strip()]


def main():
    parser = argparse.ArgumentParser(
        description="Plot palette strip from qualpal CLI output or stdin."
    )
    parser.add_argument(
        "output", type=Path, help="Output image file (e.g., palette.png)"
    )
    args = parser.parse_args()

    # Read colors from stdin
    input_data = sys.stdin.read()
    hex_colors = parse_colors(input_data)
    if not hex_colors:
        print("No colors found in input.")
        exit(1)

    fig, ax = plt.subplots(figsize=(len(hex_colors), 1))
    for i, color in enumerate(hex_colors):
        ax.add_patch(plt.Rectangle((i, 0), 1, 1, facecolor=color, edgecolor="black"))
        # determine text color based on color luminance
        r, g, b = (int(color[1:3], 16), int(color[3:5], 16), int(color[5:7], 16))
        luminance = 0.299 * r + 0.587 * g + 0.114 * b
        text_color = "black" if luminance > 128 else "white"

        ax.text(
            i + 0.5,
            0.5,
            color,
            color=text_color,
            ha="center",
            va="center",
            fontsize=8,
            fontweight="bold",
        )
    ax.set_xlim(-0.05, len(hex_colors) + 0.05)
    ax.set_ylim(-0.05, 1.05)
    ax.axis("off")
    fig.savefig(args.output, bbox_inches="tight", pad_inches=0.01, transparent=True)


if __name__ == "__main__":
    main()
