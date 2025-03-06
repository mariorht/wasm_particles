Here’s the updated **README.md**, including a section about investigating why WebAssembly runs slower than JavaScript.  

---

### **📄 README.md**
```md
# 🚀 WebAssembly Particle Simulation

A simple **C++ & WebAssembly** particle simulation with elastic collisions, rendered in the browser using JavaScript.

## 📦 Install Emscripten

```sh
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # Add this to ~/.bashrc or ~/.zshrc for persistence
```

For Fish shell:
```sh
source "/opt/projects/wasm_particles/emsdk/emsdk_env.fish"
```

## 🔧 Compile WebAssembly

```sh
emcc particle_simulation.cpp -o particles.js \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS="['_initParticles', '_updateParticles', '_getParticlePositions']" \
    -s EXPORTED_RUNTIME_METHODS="['cwrap']"
```

## 🚀 Run in Browser

Open **index.html** or start a local server:
```sh
python3 -m http.server 8080
```
Then go to:
```
http://localhost:8080/
```

## 🔍 Pending Investigation

WebAssembly is expected to be faster than JavaScript, but in this project, **the JavaScript version runs at higher FPS**. Possible reasons include:
- **Memory access overhead** when transferring data between WebAssembly and JavaScript.
- **Function call overhead** due to frequent interactions between JS and WASM.
- **WebAssembly optimizations missing** in the current compilation settings.

🚀 **Further debugging is needed to identify the exact cause.**

