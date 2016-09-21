var videoposition = 1;
var videowidth = 1;
var videoheight = 1;
var currentframe = new Image();
/*  */


document.onkeydown = checkKey;
window.onload = reloadFrame(videoposition);

function reloadAnnotations() {
  videowidth = currentframe.width;
  videoheight = currentframe.height;
  var c=document.getElementById("videocanvas");
  var ctx=c.getContext("2d");
  
  ctx.drawImage(currentframe, 0, 0);
  var canvas = document.getElementById('videocanvas');
  var context = canvas.getContext('2d');

  context.beginPath();
  context.rect(188, 50, 200, 100);
  context.fillStyle = 'yellow';
  context.fill();
  context.lineWidth = 7;
  context.strokeStyle = 'black';
  context.stroke();

  $.get("/api/annotations?frame="+videoposition, function(data, status){
	
        });

}

function drawAnnotations(annotation){
}

function addListAnnotation(annotation){
}

function reloadFrame(frame){
  currentframe.src="/image/" + frame;
  videowidth = currentframe.width;
  videoheight = currentframe.height;
}

function previousFrame(steps){
  videoposition = videoposition - steps;
  if(videoposition < 1)
    videoposition = 1;
  reloadFrame(videoposition);  
}

function nextFrame(steps){
  videoposition = videoposition + steps;
  reloadFrame(videoposition);
}

function checkKey(e) {

    e = e || window.event;

    if (e.keyCode == '38') {
        nextFrame(10);
    }
    else if (e.keyCode == '40') {
        previousFrame(10);
    }
    else if (e.keyCode == '37') {
       previousFrame(1);
    }
    else if (e.keyCode == '39') {
       nextFrame(1);
    }
}

currentframe.onload = function() {
  reloadAnnotations();
}

$('#previous').on('click', function (e) {
  previousFrame(1);
})

$('#next').on('click', function (e) {
  nextFrame(1);
})


