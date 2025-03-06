#!/bin/bash
set -v
emcc particle_simulation.cpp -o particles.js -s WASM=1 -s EXPORTED_FUNCTIONS="['_initParticles', '_updateParticles', '_getParticlePositions']" -s EXPORTED_RUNTIME_METHODS="['cwrap']"