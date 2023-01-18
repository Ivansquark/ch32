const progress = document.getElementById("progress");
const prev = document.getElementById("prev");
const next = document.getElementById("next");
const start = document.getElementById("start");
const circles = document.querySelectorAll(".circle");
let currentActive = 1;
next.addEventListener("click", () => {
    currentActive++;
    if (currentActive > circles.length) currentActive = circles.length;
    update();
});
prev.addEventListener("click", () => {
    currentActive--;
    if (currentActive < 1) currentActive = 1;
    update();
});
start.addEventListener("click", startstring)
const update = () => {
    circles.forEach((circle, index) => {
        if (index < currentActive) {
            circle.classList.add("active");
        } else {
            circle.classList.remove("active");
        }
    });
    const actives = document.querySelectorAll(".active");
    progress.style.width =
    ((actives.length - 1) / (circles.length - 1)) * 100 + "%";
    if (currentActive === 1) {
        prev.disabled = true;
        document.getElementById("info").textContent="START TEXT";
    } else if (currentActive === circles.length) {
        next.disabled = true;
        document.getElementById("info").textContent="END TEXT";
    } else {
        prev.disabled = false;
        next.disabled = false;
    }
};
var xhr;
var idTimer1, idTimer2;
var temp = false;
var add = 0;
function Timer1() {
    xhr.open("GET", "content.html?r=" + Math.random(), true); //Math.random() - защита от кеширования
    xhr.responseType = "text";
    xhr.onload = function (oEvent) {
        document.getElementById('information').innerHTML = xhr.response;
    }
    xhr.send(null);
    idTimer1 = setTimeout("Timer1()", 10);
    if(!temp) {
        temp = true;
        document.getElementById("info").textContent="END TEXT";
    } else {
        temp = false;
        document.getElementById("info").textContent="START TEXT";
    }
    draw(add++);
}
var IsStarted = false;
function startstring() {
    if(!IsStarted) {
        IsStarted = true;
        document.getElementById('start').innerHTML = 'STOP';
        Timer1();
    } else {
        IsStarted = false;
        clearTimeout(idTimer1);
        clearTimeout(idTimer2);
        document.getElementById('start').innerHTML = 'START';
        document.getElementById('information').innerHTML = "";
    }
}
function onload() {
    xhr = new(XMLHttpRequest);
}
function draw(i) {
    var V = 3*i/4096    
    var canvas = document.getElementById('myChart');
    if (canvas.getContext) {
        var ctx = canvas.getContext('2d');
        ctx.clearRect(0, 0, canvas.width, canvas.height);        
        ctx.fillStyle = "red";
        ctx.fillRect(0, 0, canvas.width*V/3, 50);        
        ctx.fillStyle = "blue";
        ctx.font = "italic 30pt Arial";        
        ctx.fillText(V.toFixed(3) + "V", 35, 150);
    }
}
