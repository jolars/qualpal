export function sliderKeyValue(
  key: string,
  current: number,
  maximum: number,
  step: number,
) {
  switch (key) {
    case "ArrowRight":
    case "ArrowUp":
      return Math.min(maximum, current + step);
    case "ArrowLeft":
    case "ArrowDown":
      return Math.max(0, current - step);
    case "Home":
      return 0;
    case "End":
      return maximum;
  }
}
