var videoposition = 0
var videowidth = 800
var videoheight = 600

var currentImage = new Image()

var renderer = PIXI.autoDetectRenderer(800, 600, {view: document.getElementById('videocanvas')})

var Container = PIXI.Container,
  autoDetectRenderer = PIXI.autoDetectRenderer,
  resources = PIXI.loader.resources,
  Sprite = PIXI.Sprite,
  Texture = PIXI.Texture

var stage = new PIXI.Container()

var annotationslist = $('#annotationslist')

renderer.view.onclick = handleClick

function reloadAnnotations () {
  annotationslist.empty()

  $.getJSON('/api/annotations?frame=' + videoposition, function (data) {
    $.each(data.annotations, function (i, annotation) {
      addListAnnotation(annotation)
      drawAnnotations(annotation)
    })
  })
}

/**
 * draw annotation rect on canvas
 */
function drawAnnotations (annotation) {
  var graphics = new PIXI.Graphics()
  graphics.lineStyle(3, 0xFF0000)
  graphics.drawRect(annotation.x, annotation.y, annotation.w, annotation.h)
  stage.addChild(graphics)
}

function addListAnnotation (annotation) {
  annotationslist.prepend('<li class="list-group-item"> ' + annotation.name + '</li>')
}

function render () {
  requestAnimationFrame(render)
  renderer.resize(videowidth, videoheight)
  renderer.render(stage)
}

function reloadFrame (frame) {
  currentImage.src = '/image/' + frame
  currentImage.onload =
    function () {
      Texture = PIXI.Texture.fromImage(this.src)
      videowidth = this.width
      videoheight = this.height
      Sprite = new PIXI.Sprite(Texture)
      Sprite.interactive = true
      // clear 
      for (var i = stage.children.length - 1; i >= 0; i--) {	stage.removeChild(stage.children[i]);}

      stage.addChild(Sprite)
      reloadAnnotations()
      render()
  }
}

function handleClick (event) {
  // window.alert("click" +event.x + "" + stage.toLocal({'x': event.x, 'y':event.y}))
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
renderer.resize(videowidth, videoheight)
renderer.backgroundColor = 0x061639
renderer.render(stage)
reloadFrame(videoposition)
