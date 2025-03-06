let particlesJS = [];

function initParticlesJS(count) {
    particlesJS = [];
    for (let i = 0; i < count; i++) {
        particlesJS.push({
            x: Math.random() * 800,
            y: Math.random() * 600,
            vx: (Math.random() * 2 - 1) * 2,
            vy: (Math.random() * 2 - 1) * 2
        });
    }
}

function updateParticlesJS(dt) {
    const gravity = 0.0;
    const damping = 1.0;
    const radius = 7.0;

    for (let p of particlesJS) {
        p.vy += gravity * dt;

        p.x += p.vx;
        p.y += p.vy;

        // Rebote en bordes
        if (p.x <= radius || p.x >= 800 - radius) {
            p.vx *= -1;
            p.x = p.x <= radius ? radius : 800 - radius;
        }
        if (p.y <= radius || p.y >= 600 - radius) {
            p.vy *= -1;
            p.y = p.y <= radius ? radius : 600 - radius;
        }
    }

    // Colisiones entre partículas con ajuste de overlap
    for (let i = 0; i < particlesJS.length; i++) {
        for (let j = i + 1; j < particlesJS.length; j++) {
            let dx = particlesJS[j].x - particlesJS[i].x;
            let dy = particlesJS[j].y - particlesJS[i].y;
            let dist = Math.sqrt(dx * dx + dy * dy);
            let minDist = 2 * radius;

            if (dist < minDist) { // Si hay colisión (overlap)
                let nx = dx / dist;
                let ny = dy / dist;

                // Separar partículas para evitar solapamiento
                let overlap = (minDist - dist) / 2;
                particlesJS[i].x -= overlap * nx;
                particlesJS[i].y -= overlap * ny;
                particlesJS[j].x += overlap * nx;
                particlesJS[j].y += overlap * ny;

                // Producto escalar para calcular velocidades en la dirección de colisión
                let vi = particlesJS[i].vx * nx + particlesJS[i].vy * ny;
                let vj = particlesJS[j].vx * nx + particlesJS[j].vy * ny;

                // Intercambio de velocidades en la dirección normal
                let temp = vi;
                vi = vj;
                vj = temp;

                // Aplicar las nuevas velocidades en la dirección de la colisión
                particlesJS[i].vx += (vi - (particlesJS[i].vx * nx + particlesJS[i].vy * ny)) * nx;
                particlesJS[i].vy += (vi - (particlesJS[i].vx * nx + particlesJS[i].vy * ny)) * ny;
                particlesJS[j].vx += (vj - (particlesJS[j].vx * nx + particlesJS[j].vy * ny)) * nx;
                particlesJS[j].vy += (vj - (particlesJS[j].vx * nx + particlesJS[j].vy * ny)) * ny;
            }
        }
    }
}

function getParticlePositionsJS() {
    let positions = [];
    for (let p of particlesJS) {
        positions.push(p.x, p.y);
    }
    return positions;
}
