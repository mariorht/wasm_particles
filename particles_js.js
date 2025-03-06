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
    const damping = 1;
    const radius = 7.0;

    for (let p of particlesJS) {
        p.vy += gravity * dt;

        p.x += p.vx;
        p.y += p.vy;

        // Rebote en bordes
        if (p.x <= radius || p.x >= 800 - radius) {
            p.vx *= -damping;
            p.x = p.x <= radius ? radius : 800 - radius;
        }
        if (p.y <= radius || p.y >= 600 - radius) {
            p.vy *= -damping;
            p.y = p.y <= radius ? radius : 600 - radius;
        }
    }

    // Colisiones entre partículas
    for (let i = 0; i < particlesJS.length; i++) {
        for (let j = i + 1; j < particlesJS.length; j++) {
            let dx = particlesJS[j].x - particlesJS[i].x;
            let dy = particlesJS[j].y - particlesJS[i].y;
            let dist = Math.sqrt(dx * dx + dy * dy);

            if (dist < 2 * radius) {
                let nx = dx / dist;
                let ny = dy / dist;

                let vi = particlesJS[i].vx * nx + particlesJS[i].vy * ny;
                let vj = particlesJS[j].vx * nx + particlesJS[j].vy * ny;

                let temp = vi;
                vi = vj;
                vj = temp;

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
