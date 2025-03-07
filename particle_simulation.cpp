#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <emscripten.h>
#include <cstdio>

struct Particle {
    float x, y;
    float vx, vy;
    float radius = 10.0f;
};

const int WIDTH = 800;
const int HEIGHT = 600;
const float DAMPING = 1.0f; // Sin pérdida de energía
const float GRAVITY = 0.0f; // Sin gravedad

std::vector<Particle> particles;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void initParticles(int count, float radius) {
        particles.clear();
        srand(time(0));
        for (int i = 0; i < count; i++) {
            Particle p;
            p.x = rand() % WIDTH;
            p.y = rand() % HEIGHT;
            p.vx = (rand() % 200 - 100) / 50.0f;  // Velocidad aleatoria en X
            p.vy = (rand() % 200 - 100) / 50.0f;  // Velocidad aleatoria en Y
            p.radius = radius;
            particles.push_back(p);
        }
    }

    EMSCRIPTEN_KEEPALIVE
    void updateParticles(float dt) {
        float totalEnergy = 0.0f;
        for (auto &p : particles) {
            p.vx *= DAMPING;
            p.vy *= DAMPING;
            
            p.x += p.vx;
            p.y += p.vy;

            // Compute kinetic energy (E = 1/2 * m * v^2) 
            // Assuming mass m = 1 for simplicity
            totalEnergy += 0.5f * (p.vx * p.vx + p.vy * p.vy);

            // Rebote en los bordes
            if (p.x <= p.radius || p.x >= WIDTH - p.radius) {
                p.vx *= -1;
                p.x = p.x <= p.radius ? p.radius : WIDTH - p.radius;
            }
            if (p.y <= p.radius || p.y >= HEIGHT - p.radius) {
                p.vy *= -1;
                p.y = p.y <= p.radius ? p.radius : HEIGHT - p.radius;
            }
        }

        // Format the energy to 3 decimal places
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%.3f", totalEnergy);

        // Print to JavaScript console
        EM_ASM({
            console.log("C++        - Energía total: " + UTF8ToString($0));
        }, buffer);

        // Detectar colisiones entre partículas
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float dist = std::sqrt(dx * dx + dy * dy);
                float minDist = particles[j].radius + particles[i].radius;

                if (dist < minDist) { // Si hay colisión
                    float nx = dx / dist;
                    float ny = dy / dist;

                    // Separar partículas para evitar solapamientos
                    float overlap = (minDist - dist) / 2;
                    particles[i].x -= overlap * nx;
                    particles[i].y -= overlap * ny;
                    particles[j].x += overlap * nx;
                    particles[j].y += overlap * ny;

                    // Producto escalar para obtener la velocidad en la dirección de la colisión
                    float vi = particles[i].vx * nx + particles[i].vy * ny;
                    float vj = particles[j].vx * nx + particles[j].vy * ny;

                    // Intercambiar velocidades en la dirección normal (conservación de energía)
                    particles[i].vx += (vj - vi) * nx;
                    particles[i].vy += (vj - vi) * ny;
                    particles[j].vx += (vi - vj) * nx;
                    particles[j].vy += (vi - vj) * ny;
                }
            }
        }
    }

    EMSCRIPTEN_KEEPALIVE
    float* getParticlePositions() {
        static std::vector<float> positions;
        positions.resize(particles.size() * 2);

        for (size_t i = 0; i < particles.size(); i++) {
            positions[i * 2] = particles[i].x;
            positions[i * 2 + 1] = particles[i].y;
        }
        return positions.data();
    }
}
