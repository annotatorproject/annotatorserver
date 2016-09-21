var videoposition = 1;
var videowidth = 1;
var videoheight = 1;
var currentframe = new Image();
var annotationslist = $('#annotationslist');
/*  */


document.onkeydown = checkKey;
window.onload = reloadFrame(videoposition);

function reloadAnnotations() {
  videowidth = currentframe.width;
  videoheight = currentframe.height;
  
  var canvas = document.getElementById('videocanvas');
  var context = canvas.getContext('2d');

  context.clearRect(0, 0, canvas.width, canvas.height);
  context.drawImage(currentframe, 0, 0);
  annotationslist.empty();

  $.getJSON( "/api/annotations?frame="+videoposition, function( data ) {
	$.each( data.annotations, function( i, annotation ) {
		addListAnnotation(annotation);
		drawAnnotations(annotation);
	});
  });


}

function drawAnnotations(annotation){
var canvas = document.getElementById('videocanvas');
  var context = canvas.getContext('2d');
  context.beginPath();
  context.rect(annotation.x, annotation.y, annotation.w, annotation.h);
  context.fillStyle = "rgba(100, 255, 100, 0.3)";
  context.fill();
  context.lineWidth = 3;
  context.strokeStyle = 'black';
  context.stroke();
}

function addListAnnotation(annotation){
annotationslist.prepend('<li class="list-group-item"> ' + annotation.name + '</li>');
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



