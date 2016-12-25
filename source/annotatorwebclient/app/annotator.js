var videoposition = 0
var videowidth = 1
var videoheight = 1

var renderer = PIXI.autoDetectRenderer(800, 600, {view: document.getElementById("videocanvas")})

var Container = PIXI.Container,
  autoDetectRenderer = PIXI.autoDetectRenderer,
  resources = PIXI.loader.resources,
  Sprite = PIXI.Sprite

//document.getElementById('videocanvas').appendChild(renderer.view)
var stage = new PIXI.Container()

var annotationslist = $('#annotationslist')

function reloadAnnotations () {
  videowidth = currentframe.width
  videoheight = currentframe.height

  var canvas = document.getElementById('videocanvas')
  var context = canvas.getContext('2d')

  context.clearRect(0, 0, canvas.width, canvas.height)
  context.drawImage(currentframe, 0, 0)
  annotationslist.empty()

  $.getJSON('/api/annotations?frame=' + videoposition, function (data) {
    $.each(data.annotations, function (i, annotation) {
      addListAnnotation(annotation)
      drawAnnotations(annotation)
    })
  })
}

function drawAnnotations (annotation) {
  var canvas = document.getElementById('videocanvas')
  var context = canvas.getContext('2d')
  context.beginPath()
  context.rect(annotation.x, annotation.y, annotation.w, annotation.h)
  context.fillStyle = 'rgba(100, 255, 100, 0.3)'
  context.fill()
  context.lineWidth = 3
  context.strokeStyle = 'black'
  context.stroke()
}

function addListAnnotation (annotation) {
  annotationslist.prepend('<li class="list-group-item"> ' + annotation.name + '</li>')
}

function render(){
  requestAnimationFrame(render);
  renderer.render(stage);
}

function reloadFrame (frame) {
  var texture = PIXI.Texture.fromImage('/image/' + frame);
  var sprite = new PIXI.Sprite(texture);
  
  // clear 
  for (var i = stage.children.length - 1; i >= 0; i--) {	stage.removeChild(stage.children[i]);};

  stage.addChild(sprite);
  render();
}

function previousFrame (steps) {
  videoposition = videoposition - steps
  if (videoposition < 0)
    videoposition = 0
  reloadFrame(videoposition)
}

function nextFrame (steps) {
  videoposition = videoposition + steps
  reloadFrame(videoposition)
}

$('#previous').on('click', function (e) {
  previousFrame(1)
})

$('#next').on('click', function (e) {
  nextFrame(1)
})

function keyboard (keyCode) {
  var key = {}
  key.code = keyCode
  key.isDown = false
  key.isUp = true
  key.press = undefined
  key.release = undefined
  // The `downHandler`
  key.downHandler = function (event) {
    if (event.keyCode === key.code) {
      if (key.isUp && key.press) key.press()
      key.isDown = true
      key.isUp = false
    }
    event.preventDefault()
  }

  // The `upHandler`
  key.upHandler = function (event) {
    if (event.keyCode === key.code) {
      if (key.isDown && key.release) key.release()
      key.isDown = false
      key.isUp = true
    }
    event.preventDefault()
  }

  // Attach event listeners
  window.addEventListener(
    'keydown', key.downHandler.bind(key), false
  )
  window.addEventListener(
    'keyup', key.upHandler.bind(key), false
  )
  return key
}

var left = keyboard(37),
  up = keyboard(38),
  right = keyboard(39),
  down = keyboard(40)

// Left arrow key `press` method
left.press = function () {
  previousFrame(1)
}

// Left arrow key `release` method
left.release = function () {}

// Up
up.press = function () {
  nextFrame(10)
}
up.release = function () {}

// Right
right.press = function () {
  nextFrame(1)
}
right.release = function () {}

// Down
down.press = function () {
  previousFrame(10)
}
down.release = function () {}

renderer.view.style.border = '1px dashed black'
renderer.resize(800, 600)
renderer.backgroundColor = 0x061639
renderer.render(stage)
reloadFrame(videoposition);