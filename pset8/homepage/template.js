

// In use
class navbar extends HTMLElement {
    connectedCallback() {
      this.innerHTML =`
      <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
          <div class="container-fluid">
              <a class="navbar-brand" href="#">Site Links</a>
              <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
              <span class="navbar-toggler-icon"></span>
              </button>
              <div class="collapse navbar-collapse" id="navbarNav">
                  <ul class="navbar-nav">
                      <li class="nav-item">
                          <a class="nav-link active" aria-current="page" href="/index.html">Home</a>
                      </li>
                      <li class="nav-item">
                          <a class="nav-link" href="/gunpla/gunpla.html">Gunpla</a>
                      </li>
                      <li class="nav-item">
                          <a class="nav-link" href="/cosplay/cosplay.html">Cosplay</a>
                      </li>
                      <li class="nav-item">
                          <a class="nav-link" href="/coding/coding.html">Coding</a>
                      </li>
                  </ul>
              </div>
          </div>
      </nav>
    `;
    }
  }

// define this as an element
customElements.define('nav-bar', navbar);