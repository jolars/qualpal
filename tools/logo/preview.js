import * as THREE from "three";
import { Line2 } from "three/addons/lines/Line2.js";
import { LineGeometry } from "three/addons/lines/LineGeometry.js";
import { LineMaterial } from "three/addons/lines/LineMaterial.js";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

async function preview() {
  const response = await fetch("../../build/logo.json");
  if (!response.ok) {
    throw new Error("Generate build/logo.json with build/generate_logo --json.");
  }
  const points = (await response.json()).filter(
    (point, index) => point.selected || index % 5 !== 0,
  );
  const container = document.querySelector("#scene");
  const scene = new THREE.Scene();
  const camera = new THREE.PerspectiveCamera(43, 1, 1, 500);
  camera.position.set(65, 55, -128);

  const renderer = new THREE.WebGLRenderer({ alpha: true, antialias: true });
  container.append(renderer.domElement);

  const controls = new OrbitControls(camera, renderer.domElement);
  controls.target.set(0, 1, 0);
  controls.minDistance = 85;
  controls.maxDistance = 290;
  controls.update();
  controls.saveState();

  const themeButton = document.querySelector("#theme");
  let dark = matchMedia("(prefers-color-scheme: dark)").matches;
  const axisMaterial = new LineMaterial({ linewidth: 1.35 });
  const sphere = new THREE.SphereGeometry(1, 28, 18);
  const markers = points.map(({ coordinates: [a, b, lightness], hex, selected }) => {
    const material = new THREE.ShaderMaterial({
      uniforms: {
        baseColor: { value: new THREE.Color(hex) },
        highlightStrength: { value: selected ? 0.58 : 0.22 },
      },
      vertexShader: `
        varying vec3 surfaceNormal;
        void main() {
          surfaceNormal = normalize(normalMatrix * normal);
          gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
        }
      `,
      fragmentShader: `
        uniform vec3 baseColor;
        uniform float highlightStrength;
        varying vec3 surfaceNormal;

        void main() {
          vec3 n = normalize(surfaceNormal);
          vec3 light = normalize(vec3(-0.55, 0.75, 0.65));
          float incidence = dot(n, light);

          // Use narrow transitions to keep the bands crisp at small sizes.
          float edge = max(fwidth(incidence), 0.005);
          float midtone = smoothstep(-0.18 - edge, -0.18 + edge, incidence);
          float lit = smoothstep(0.42 - edge, 0.42 + edge, incidence);
          vec3 shadow = baseColor * vec3(0.33, 0.39, 0.56);
          vec3 color = mix(shadow, baseColor * 0.70, midtone);
          color = mix(color, baseColor, lit);

          vec3 halfway = normalize(light + vec3(0.0, 0.0, 1.0));
          float reflection = dot(n, halfway);
          float glint = smoothstep(0.976, 0.985, reflection);
          color = mix(color, vec3(1.0, 0.96, 0.90), glint * highlightStrength);

          gl_FragColor = vec4(color, 1.0);
          #include <colorspace_fragment>
        }
      `,
    });
    const mesh = new THREE.Mesh(sphere, material);
    mesh.position.set(a, lightness - 58, -b);
    mesh.scale.setScalar(selected ? 4.6 : 1.55);
    scene.add(mesh);
    return { material, hex, selected };
  });

  for (const [axis, length] of [
    [new THREE.Vector3(1, 0, 0), 45],
    [new THREE.Vector3(0, 0, -1), 45],
    [new THREE.Vector3(0, 1, 0), 49],
  ]) {
    const end = axis.multiplyScalar(length);
    const start = end.clone().negate();
    const geometry = new LineGeometry().setPositions([...start, ...end]);
    scene.add(new Line2(geometry, axisMaterial));
  }

  function draw() {
    renderer.render(scene, camera);
  }

  function resize() {
    const size = container.clientWidth;
    axisMaterial.linewidth = size * 0.0045;
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    renderer.setSize(size, size, false);
    draw();
  }

  function applyTheme() {
    document.documentElement.classList.toggle("dark", dark);
    themeButton.textContent = dark ? "Light preview" : "Dark preview";
    axisMaterial.color.set(dark ? "#e7edf7" : "#142136");
    const background = new THREE.Color(dark ? "#121212" : "#ffffff");
    for (const { material, hex, selected } of markers) {
      const color = material.uniforms.baseColor.value.set(hex);
      if (!selected) {
        color.lerp(background, dark ? 0.70 : 0.75);
      }
    }
    draw();
  }

  controls.addEventListener("change", draw);
  new ResizeObserver(resize).observe(container);
  document.querySelector("#reset").addEventListener("click", () => controls.reset());
  themeButton.addEventListener("click", () => {
    dark = !dark;
    applyTheme();
  });
  document.querySelector("#export").addEventListener("click", () => {
    renderer.setPixelRatio(1);
    axisMaterial.linewidth = 1200 * 0.0045;
    renderer.setSize(1200, 1200, false);
    draw();
    renderer.domElement.toBlob((blob) => {
      if (!blob) return;
      const url = URL.createObjectURL(blob);
      const link = document.createElement("a");
      link.href = url;
      link.download = dark ? "qualpal-logo-dark.png" : "qualpal-logo.png";
      link.click();
      setTimeout(() => URL.revokeObjectURL(url), 1000);
    });
    resize();
  });
  resize();
  applyTheme();
}

preview().catch((error) => {
  const status = document.querySelector("#status");
  status.textContent = error.message;
  status.hidden = false;
});
