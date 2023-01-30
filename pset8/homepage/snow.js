
let addSnow = () => {
const random = (min, max) => Math.random() * (max - min) + min;
let screenWidth = window.innerWidth;
let screenHeight = window.innerHeight;

let snow = document.createElement('div');
    snow.style.position = "fixed";
    snow.style.top = "-2px";
    snow.style.right = random(0, screenWidth) + "px";
    snow.style.width = "3px";
    snow.style.height = "3px";
    snow.style.backgroundColor = "#fff";
    snow.style.borderRadius = "80%";
    snow.style.opacity = (Math.random()*4).toString()
    snow.style.zIndex = "999";
    snow.style.pointerEvents = "none";

const animateSnow = () => {
    snow.style.top = parseInt(snow.style.top) + 2 + "px";
    snow.style.right = parseInt(snow.style.right) + random(0, 2) + "px";
    /**
     * If it's out of the screen, move it to the top
     * and randomize its position
     **/
    if (parseInt(snow.style.top) > screenHeight) {
        snow.style.right = random(0, screenWidth) + "px";
        snow.style.top = parseInt("-" + random(0, 20) + "px");
    }
    window.requestAnimationFrame(animateSnow);
};
window.requestAnimationFrame(animateSnow);
document.body.appendChild(snow);
};

for (let i = 0; i < 400; i++) {
    setTimeout(addSnow, i * 200);
}
