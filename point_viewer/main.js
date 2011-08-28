var gl;
var draw_eps = 0.05, angle_eps = 0.1;
var cube_buffers = [];
var triangleVertexPositionBuffer, squareVertexPositionBuffer;
var xAngle = 0, yAngle = 0, zAngle = 0;
var zoom = -11;
var zoom_eps = 0.5;
var location_eps = 0.5;
var x_deviation = 0 , y_deviation = 0, z_deviation = -11;
var triangles = [];
function applyRotation(mat){
  mat4.rotate(mat, xAngle, [1,0,0]);
  mat4.rotate(mat, yAngle, [0,1,0]);
  mat4.rotate(mat, zAngle, [0,0,1]); 
}
function initGL(canvas) {
    try {
        gl = canvas.getContext("experimental-webgl");
        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;
    } catch (e) {
    }
    if (!gl) {
        alert("Could not initialise WebGL, sorry :-(");
    }
}


function getShader(gl, id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
        return null;
    }

    var str = "";
    var k = shaderScript.firstChild;
    while (k) {
        if (k.nodeType == 3) {
            str += k.textContent;
        }
        k = k.nextSibling;
    }

    var shader;
    if (shaderScript.type == "x-shader/x-fragment") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == "x-shader/x-vertex") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
        return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}


var shaderProgram;

function initShaders() {
    var fragmentShader = getShader(gl, "shader-fs");
    var vertexShader = getShader(gl, "shader-vs");

    shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert("Could not initialise shaders");
    }

    gl.useProgram(shaderProgram);

    shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
    gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);


    shaderProgram.vertexColorAttribute = gl.getAttribLocation(shaderProgram, "aVertexColor");
    gl.enableVertexAttribArray(shaderProgram.vertexColorAttribute);


    shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
    shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
}


var mvMatrix = mat4.create();
var pMatrix = mat4.create();

function setMatrixUniforms() {
    gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
    gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}



var triangleVertexPositionBuffer;
var squareVertexPositionBuffer;
var points = [];
function createFaceBuffer(i, sgn){
  var j, k;
  var verticies = [];
  for (j=0;j<=3; j++)
    verticies.push([0,0,0]);

  for (j=0;j<=3; j++){
    verticies[j][i] += sgn * draw_eps;
  }
    
  for (j=0;j<=2;j++)
    if (j!=i){
      verticies[0][j] += draw_eps;
      verticies[3][j] -= draw_eps
    }
  var cnt = 0;
  for (j=0;j<=2;j++)
    if (j!=i){
      if (!cnt){
        verticies[1][j] += draw_eps;
        verticies[2][j] -= draw_eps;
        cnt++;
      }
      else{
        verticies[1][j] -= draw_eps;
        verticies[2][j] += draw_eps;
      }
    }
  console.log(verticies);
  var vec_verticies = []
  
  for (j=0;j<=3;j++)
    for (k=0;k<=2;k++)
      vec_verticies.push(verticies[j][k]);

  var buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vec_verticies), gl.STATIC_DRAW);
  
  buffer.numItems = 4;
  buffer.itemSize = 3;
  
  cube_buffers.push(buffer);
}
function initBuffers() {
  var i;
  for (i=0; i<=2; i++){
    createFaceBuffer(i, -1);
    createFaceBuffer(i, +1);
  }
   triangleVertexPositionBuffer = gl.createBuffer();
/*    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexPositionBuffer);
    var vertices = [
         0.0,  1.0,  0.0,
        -1.0, -1.0,  0.0,
         1.0, -1.0,  0.0
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    triangleVertexPositionBuffer.itemSize = 3;
    triangleVertexPositionBuffer.numItems = 3;

    squareVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    vertices = [
         1.0,  1.0,  0.0,
        -1.0,  1.0,  0.0,
         1.0, -1.0,  0.0,
        -1.0, -1.0,  0.0
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    squareVertexPositionBuffer.itemSize = 3;
    squareVertexPositionBuffer.numItems = 4;*/
}


function addPoint(x, y, z, r, g, b){
  var coords, i, j;
  var point = new Object();
  point.coords = [x, y, z];
  point.colors = [r, g, b, 1.0];
  point.color_buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, point.color_buffer);
  var colors = [];
  for (j=0; j<=3; j++)
    colors = colors.concat(point.colors);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
  
  points.push(point);
}

function drawScene() {
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

    mat4.identity(mvMatrix);
    var i,j;
    console.log('Incep sa afisez');
    for (i=0; i<points.length; i++){
      console.log(points[i].coords);
      mat4.identity(mvMatrix);
      mat4.translate(mvMatrix, [x_deviation, y_deviation, z_deviation]);
      applyRotation(mvMatrix);
      mat4.translate(mvMatrix, points[i].coords);
     for (j=0; j<cube_buffers.length; j++){
        gl.bindBuffer(gl.ARRAY_BUFFER, points[i].color_buffer);
        gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, 4, gl.FLOAT, false, 0,0);


        gl.bindBuffer(gl.ARRAY_BUFFER, cube_buffers[j]);
        gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, 3, gl.FLOAT, false, 0,0);
        
        setMatrixUniforms()
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
      }
    }
    for (i=0; i<triangles.length; i++){
      mat4.identity(mvMatrix);
      mat4.translate(mvMatrix, [x_deviation,y_deviation,z_deviation]);
      applyRotation(mvMatrix);

      gl.bindBuffer(gl.ARRAY_BUFFER, triangles[i].color_buffer);
      gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, 4, gl.FLOAT, false, 0, 0);

      gl.bindBuffer(gl.ARRAY_BUFFER, triangles[i].position_buffer);
      gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, 3, gl.FLOAT, false, 0, 0);

      setMatrixUniforms();
      gl.drawArrays(gl.TRIANGLES, 0, 3); 
    }
/*    mat4.identity(mvMatrix);
    mat4.translate(mvMatrix, [-1.5, 0.0, -7.0]);
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, triangleVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
)setMatrixUniforms();
    gl.drawArrays(gl.TRIANGLES, 0, triangleVertexPositionBuffer.numItems);

)
    mat4.translate(mvMatrix, [3.0, 0.0, 0.0]);
    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
    setMatrixUniforms();
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, squareVertexPositionBuffer.numItems);*/
}
function rand(){
  return Math.floor(Math.random()*256);
}
function handleAddPoint(evt){
  evt.preventDefault();
  evt.stopPropagation();
  var form = $(evt.target).parent('form');
  var x,y,z;
  x = parseFloat(form.find('.x').val());
  y = parseFloat(form.find('.y').val());
  z = parseFloat(form.find('.z').val());
  var r = rand(), g=rand(), b = rand();
  addPoint(x,y,z,r/255,g/255, b/255);

  $('#point-container').append($('<li><span class="point-color" style="background-color: rgb('+r+','+g+','+b+')"></span> Coords: '+x+', '+y+', '+z+'</li>'));
  drawScene();

}

function webGLStart() {
    var canvas = document.getElementById("canvas");
    initGL(canvas);
    initShaders();
    initBuffers();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);

    drawScene();
}
function handleAngleChange(evt){
  var button = $(evt.target);
  eval(button.attr('rel') + "+=" + button.attr('sgn') + '1 *' + angle_eps);
  drawScene();
}
function handleZoomChange(evt){
  var button = $(evt.target);
  zoom += eval(button.attr('sgn') + '1*' + zoom_eps);
  console.log(zoom);
  drawScene();
}

function generateTriangleBuffers(triangle){
  triangle.position_buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, triangle.position_buffer);
  var verticies = [];
  var i;
  for (i=0;i<triangle.points.length;i++){
    console.log(triangle.points[i]);
    verticies = verticies.concat(points[triangle.points[i]].coords);
  }
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verticies), gl.STATIC_DRAW);

  triangle.color_buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, triangle.color_buffer);
  var colors = [];
  for (i=0; i<triangle.points.length;i++)
    colors = colors.concat(points[triangle.points[i]].colors);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
}
function handleAddTriangle(evt){
  evt.preventDefault();
  evt.stopPropagation();
  var form = $(evt.target).parent('form');
  var inputs = form.find('input[type="text"]');
  var i;
  var triangle = new Object();
  triangle.points = [];
  for (i=0;i<inputs.length;i++){
    triangle.points.push(parseInt($(inputs[i]).val()) - 1);
  }
  generateTriangleBuffers(triangle);
  triangles.push(triangle);
  drawScene();
}
function changeLocation(evt){
  console.log(evt);
  if (evt.charCode == 119){
    z_deviation += location_eps;
    //console.log(z_deviation);
    drawScene();
    //evt.preventDefault();
  }
  if (evt.charCode == 115){
    z_deviation -= location_eps;
    drawScene();
    //evt.preventDefault();
  }
  if (evt.charCode == 97){
    x_deviation += location_eps;
    drawScene();
    //evt.preventDefault();
  }
  if (evt.charCode == 100){
    x_deviation -= location_eps;
    drawScene();
    //evt.preventDefault();
  }
  if (evt.charCode == 113){
    y_deviation += location_eps;
    drawScene();
    //evt.preventDefault();
  }
  if (evt.charCode == 101){
    y_deviation -= location_eps;
   // evt.preventDefault();
    drawScene();
  }
}
function startup(){
  $('#add-point-button').click(handleAddPoint);
  $('.change-angle').click(handleAngleChange);
  $('.change-zoom').click(handleZoomChange);
  $('#add-triangle-button').click(handleAddTriangle);
  $(document).keypress(changeLocation);
  webGLStart();
}
