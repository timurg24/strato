# Shaders Documentation

## Shader creation

Shaders are normal BGFX (GLSL-style) shaders but include a payload to define the uniforms.

## Global uniforms

These uniforms are available to any shader:

- `cameraPos` - Vec4
- `sunDirection` - Vec4
  - `w` - Light count
- `sunColor` - Vec4
  - `w` - Sun intensity
- `pointPosition` - Array of Vec4 (size of Light count or `sunDirection.w`)
- `pointMath` - Array of Vec4 (size of Light count or `sunDirection.w`)
  - `x` - Range
  - `y` - Intensity
- `pointColor` - Array of Vec4 (size of Light count or `sunDirection.w`)
