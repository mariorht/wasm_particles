#include <vector>
#include <cstdlib>
#include <ctime>
#include <emscripten.h>

struct Particle {
    float x, y;
    float vx, vy;
};

const int WIDTH = 800;
const int HEIGHT = 600;
const float GRAVITY = 00.0f;
const float DAMPING = 1.0f;  // Factor de rebote

std::vector<Particle> particles;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void initParticles(int count) {
        particles.clear();
        srand(time(0));
        for (int i = 0; i < count; i++) {
            Particle p;
            p.x = WIDTH / 2.0f;
            p.y = HEIGHT / 2.0f;
            p.vx = (rand() % 200 - 100) / 100.0f;  // Velocidad aleatoria en X
            p.vy = (rand() % 200 - 100) / 100.0f;  // Velocidad aleatoria en Y
            particles.push_back(p);
        }
    }

    EMSCRIPTEN_KEEPALIVE
    void updateParticles(float dt) {
        for (auto &p : particles) {
            p.vy += GRAVITY * dt;  // Aplicar gravedad

            p.x += p.vx;
            p.y += p.vy;

            // Rebote en los bordes
            if (p.x <= 0 || p.x >= WIDTH) {
                p.vx *= -DAMPING; // Invertir velocidad con amortiguación
                p.x = p.x <= 0 ? 0 : WIDTH;
            }
            if (p.y <= 0 || p.y >= HEIGHT) {
                p.vy *= -DAMPING;
                p.y = p.y <= 0 ? 0 : HEIGHT;
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
