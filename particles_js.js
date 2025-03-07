let particlesJS = [];

function initParticlesJS(count, radius) {
    particlesJS = [];
    for (let i = 0; i < count; i++) {
        particlesJS.push({
            x: Math.random() * 800,
            y: Math.random() * 600,
            vx: (Math.random() * 2 - 1) * 2,
            vy: (Math.random() * 2 - 1) * 2,
            radius: radius
        });
    }
}

function updateParticlesJS(dt) {
    const gravity = 0.0;
    let totalEnergy = 0; // Variable para almacenar la energía total del sistema

    for (let p of particlesJS) {
        p.vy += gravity * dt;

        p.x += p.vx;
        p.y += p.vy;

        // Cálculo de la energía cinética de cada partícula
        totalEnergy += 0.5 * (p.vx * p.vx + p.vy * p.vy);

        // Rebote en bordes
        if (p.x <= p.radius || p.x >= 800 - p.radius) {
            p.vx *= -1;
            p.x = p.x <= p.radius ? p.radius : 800 - p.radius;
        }
        if (p.y <= p.radius || p.y >= 600 - p.radius) {
            p.vy *= -1;
            p.y = p.y <= p.radius ? p.radius : 600 - p.radius;
        }
    }

    // Colisiones entre partículas con ajuste de overlap
    for (let i = 0; i < particlesJS.length; i++) {
        for (let j = i + 1; j < particlesJS.length; j++) {
            let dx = particlesJS[j].x - particlesJS[i].x;
            let dy = particlesJS[j].y - particlesJS[i].y;
            let dist = Math.sqrt(dx * dx + dy * dy);
            let minDist = particlesJS[j].radius + particlesJS[i].radius;

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

                // Intercambiar las velocidades en la dirección normal (correctamente)
                particlesJS[i].vx += (vj - vi) * nx;
                particlesJS[i].vy += (vj - vi) * ny;
                particlesJS[j].vx += (vi - vj) * nx;
                particlesJS[j].vy += (vi - vj) * ny;
            }
        }
    }

    // Log de la energía total del sistema en cada iteración
    console.log(`Javascript - Energía total: ${totalEnergy.toFixed(3)}`);
}



function getParticlePositionsJS() {
    let positions = [];
    for (let p of particlesJS) {
        positions.push(p.x, p.y);
    }
    return positions;
}
