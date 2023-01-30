
customElements.define('my-header', Header)

class Navbar extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        <nav class="navbar navbar-expand-lg navbar-light bg-light">
        <div class="container-fluid">
          <a class="navbar-brand" href="#">Navbar</a>
          <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
          </button>
          <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav">
              <li class="nav-item">
                <a class="nav-link active" aria-current="page" href="#">Home</a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">Features</a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">Pricing</a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">Whatever</a>
              </li>
            </ul>
          </div>
        </div>
      </nav>
    }
}

customElements.define('my-navbar', Navbar);


let addSnow = () => {
  const random = (min, max) => Math.random() * (max - min) + min;
  let screenWidth = window.innerWidth;
  let screenHeight = window.innerHeight;

  let snow = document.createElement('div');

  snow.style.position = "fixed";
  snow.style.top = "-2px";
  snow.style.right = random(0, screenWidth) + "px";
  snow.style.width = "10px";
  snow.style.height = "10px";
  snow.style.backgroundColor = "#fff";
  snow.style.borderRadius = "50%";
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

for (let i = 0; i < 60; i++) {
    setTimeout(addSnow, i * 100);
}