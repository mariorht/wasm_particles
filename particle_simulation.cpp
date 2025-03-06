#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <emscripten.h>

struct Particle {
    float x, y;
    float vx, vy;
};

const int WIDTH = 800;
const int HEIGHT = 600;
const float RADIUS = 7.0f;  // Radio de cada partícula
const float DAMPING = 1.0f; // Sin pérdida de energía
const float GRAVITY = 0.0f; // Sin gravedad

std::vector<Particle> particles;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void initParticles(int count) {
        particles.clear();
        srand(time(0));
        for (int i = 0; i < count; i++) {
            Particle p;
            p.x = rand() % WIDTH;
            p.y = rand() % HEIGHT;
            p.vx = (rand() % 200 - 100) / 50.0f;  // Velocidad aleatoria en X
            p.vy = (rand() % 200 - 100) / 50.0f;  // Velocidad aleatoria en Y
            particles.push_back(p);
        }
    }

    EMSCRIPTEN_KEEPALIVE
    void updateParticles(float dt) {
        for (auto &p : particles) {
            p.vx *= DAMPING;
            p.vy *= DAMPING;
            
            p.x += p.vx;
            p.y += p.vy;

            // Rebote en los bordes
            if (p.x <= RADIUS || p.x >= WIDTH - RADIUS) {
                p.vx *= -1;
                p.x = p.x <= RADIUS ? RADIUS : WIDTH - RADIUS;
            }
            if (p.y <= RADIUS || p.y >= HEIGHT - RADIUS) {
                p.vy *= -1;
                p.y = p.y <= RADIUS ? RADIUS : HEIGHT - RADIUS;
            }
        }

        // Detectar colisiones entre partículas
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < 2 * RADIUS) { // Si están tocándose
                    float nx = dx / dist;
                    float ny = dy / dist;

                    // Producto escalar (proyección de velocidades en la dirección de la colisión)
                    float vi = particles[i].vx * nx + particles[i].vy * ny;
                    float vj = particles[j].vx * nx + particles[j].vy * ny;

                    // Intercambio de velocidades en la dirección normal
                    float temp = vi;
                    vi = vj;
                    vj = temp;

                    // Aplicar las nuevas velocidades en la dirección de la colisión
                    particles[i].vx += (vi - (particles[i].vx * nx + particles[i].vy * ny)) * nx;
                    particles[i].vy += (vi - (particles[i].vx * nx + particles[i].vy * ny)) * ny;
                    particles[j].vx += (vj - (particles[j].vx * nx + particles[j].vy * ny)) * nx;
                    particles[j].vy += (vj - (particles[j].vx * nx + particles[j].vy * ny)) * ny;

                    // Separar partículas para evitar solapamientos
                    float overlap = (2 * RADIUS - dist) / 2;
                    particles[i].x -= overlap * nx;
                    particles[i].y -= overlap * ny;
                    particles[j].x += overlap * nx;
                    particles[j].y += overlap * ny;
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
