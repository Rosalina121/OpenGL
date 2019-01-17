var gl_canvas;
var gl_ctx;

var _triangleVertexBuffer;
var _triangleFacesBuffer;
var _position;
var _uv;
var _sampler;
var _PosMatrix;
var _MovMatrix;
var _ViewMatrix;
var _matrixProjection;
var _matrixMovement;
var _matrixView;
var _cubeTexture;

var running = false;

var rotationSpeed = 0.001;
var zoomRatio = -6;

var X, Y, Z;
var img1, img2, img3;

function runWebGL () {
   getRotation();
   getImage();
   rotationSpeed = 0.001;

   gl_canvas = document.getElementById("glcanvas");
   gl_ctx = gl_getContext(gl_canvas);
   gl_initShaders();
   gl_initBuffers();
   
   if(!running){
      gl_setMatrix();
   }  
   _cubeTexture = gl_initTexture();
   gl_draw();
}

function getRotation() {
   X = document.getElementById('rotateX').checked;
   Y = document.getElementById('rotateY').checked;
   Z = document.getElementById('rotateZ').checked;
}

function getImage() {
   img1 = document.getElementById('img1').checked;
   img2 = document.getElementById('img2').checked;
   img3 = document.getElementById('img3').checked;
}

// ==================================================================== //

function gl_getContext (canvas) {
   try {
      var ctx = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
      ctx.viewportWidth = canvas.width;
      ctx.viewportHeight = canvas.height;
   } catch (e) {}

   if (!ctx) {
      document.write('Unable to initialize WebGL. Your browser may not support it.')
   }
   return ctx;
}

// ==================================================================== //

// Declare the shaders. They are pieces of code compiled by WebGL and
// executed on the graphics device. They are written in GLSL.
function gl_initShaders () {
   // position of the point - 0. is Z and 1. is W
   // PosMatrix is uniform variable - its value is constant while rendering an object
   // MovMatrix is the movement matrix of the triangle
   // gl_position -> we move position with MovMatrix before projecting it
   var vertexShader = "\n\
      attribute vec3 position;\n\
      uniform mat4 PosMatrix;\n\
      uniform mat4 MovMatrix;\n\
      uniform mat4 ViewMatrix; \n\
      attribute vec2 uv;\n\
      varying vec2 vUV;\n\
      void main(void) {\n\
         gl_Position = PosMatrix * ViewMatrix * MovMatrix * vec4(position, 1.);\n\
         vUV = uv;\n\
      }";

   // set black color
   var fragmentShader = "\n\
      precision mediump float;\n\
      uniform sampler2D sampler;\n\
      varying vec2 vUV;\n\
      void main(void) {\n\
         gl_FragColor = texture2D(sampler, vUV);\n\
      }";

   // this function is used to compile a shader
   var getShader = function(source, type, typeString) {
      var shader = gl_ctx.createShader(type);
      gl_ctx.shaderSource(shader, source);
      gl_ctx.compileShader(shader);

      if (!gl_ctx.getShaderParameter(shader, gl_ctx.COMPILE_STATUS)) {
         alert('error in' + typeString);
         return false;
      }
      return shader;
   };

   // Compile the vertex and fragment shaders
   var shader_vertex = getShader(vertexShader, gl_ctx.VERTEX_SHADER, "VERTEX");
   var shader_fragment = getShader(fragmentShader, gl_ctx.FRAGMENT_SHADER, "FRAGMENT");


   // Create the Shader program.
   // Shader program is a combination of a vertex and fragment shaders.
   var SHADER_PROGRAM = gl_ctx.createProgram();
   gl_ctx.attachShader(SHADER_PROGRAM, shader_vertex);
   gl_ctx.attachShader(SHADER_PROGRAM, shader_fragment);


   // Linking of the shader program to the WebGL context - gl_ctx,
   // in order to match the shader variables to javascript variables
   gl_ctx.linkProgram(SHADER_PROGRAM);

   // Link PosMatrix\MovMatrix\ViewMatrix GLSL variables to
   // _PosMatrix\_MovMatrix\_ViewMatrix javascript variables
   // Uniforms do not need to be enabled like attributes
   _PosMatrix = gl_ctx.getUniformLocation(SHADER_PROGRAM, "PosMatrix");
   _MovMatrix = gl_ctx.getUniformLocation(SHADER_PROGRAM, "MovMatrix");
   _ViewMatrix = gl_ctx.getUniformLocation(SHADER_PROGRAM, "ViewMatrix");

   // position GLSL variable links to _position variable

   _sampler = gl_ctx.getUniformLocation(SHADER_PROGRAM, "sampler");
   _uv = gl_ctx.getAttribLocation(SHADER_PROGRAM, "uv");
   _position = gl_ctx.getAttribLocation(SHADER_PROGRAM, "position");    // *******
   // enable GLSL attributes variables
   gl_ctx.enableVertexAttribArray(_uv);
   gl_ctx.enableVertexAttribArray(_position);

   // linking is over - tells WebGL context to use SHADER_PROGRAM for rendering.
   gl_ctx.useProgram(SHADER_PROGRAM);
   gl_ctx.uniform1i(_sampler, 0);
}

// ==================================================================== //

function gl_initBuffers () {
   // Point coordinates array of the triangle
//   var triangleVertices = [
//      -1, -1, 0,    // bottom left
//       0, 0, 1,      // submit color: blue
//       1, -1, 0,    // bottom right
//       1, 1, 1,      // submit color: white
//       1, 1, 0,     // top right
//       1, 0, 0       // submit color: red
//   ];
   var triangleVertices = [
      -1, -1, -1,    0, 0,
      1, -1, -1,    1, 0,
      1,  1, -1,    1, 1,
      -1,  1, -1,    0, 1,

      -1, -1, 1,    0, 0,
      1, -1, 1,    1, 0,
      1,  1, 1,    1, 1,
      -1,  1, 1,    0, 1,

      -1, -1, -1,    0, 0,
      -1, 1, -1,    1, 0,
      -1,  1, 1,    1, 1,
      -1,  -1, 1,    0, 1,

      1, -1, -1,    0, 0,
      1, 1, -1,    1, 0,
      1,  1, 1,    1, 1,
      1,  -1, 1,    0, 1,

      -1, -1, -1,    0, 0,
      -1, -1, 1,    1, 0,
      1,  -1, 1,    1, 1,
      1,  -1, -1,    0, 1,

      -1, 1, -1,    0, 0
      -1, 1, 1,    1, 0,
      1,  1, 1,    1, 1,
      1,  1, -1,    0, 1
   ];


   // Building Vertex Buffer Object - WebGL vertex array
   _triangleVertexBuffer = gl_ctx.createBuffer();                // *******
   gl_ctx.bindBuffer(gl_ctx.ARRAY_BUFFER, _triangleVertexBuffer);
   gl_ctx.bufferData(gl_ctx.ARRAY_BUFFER, new Float32Array(triangleVertices), gl_ctx.STATIC_DRAW);


   // Triangle faces array
   // var triangleFaces = [0, 1, 2];
   var triangleFaces = [
      0,1,2,
      0,2,3,

      4,5,6,
      4,6,7,

      8,9,10,
      8,10,11,

      12,13,14,
      12,14,15,

      16,17,18,
      16,18,19,

      20,21,22,
      20,22,23
   ];

   _triangleFacesBuffer = gl_ctx.createBuffer();                     // *******
   gl_ctx.bindBuffer(gl_ctx.ELEMENT_ARRAY_BUFFER, _triangleFacesBuffer);
   gl_ctx.bufferData(gl_ctx.ELEMENT_ARRAY_BUFFER, new Uint16Array(triangleFaces), gl_ctx.STATIC_DRAW);
}

// ==================================================================== //

function gl_setMatrix () {
   _matrixProjection = MATRIX.getProjection(40, gl_canvas.width/gl_canvas.height, 1, 100);
   _matrixMovement = MATRIX.getIdentityMatrix();
   _matrixView = MATRIX.getIdentityMatrix();

   MATRIX.translateZ(_matrixView, zoomRatio);
}

// ==================================================================== //

function gl_initTexture() {
   var img = new Image();

   
   if(img1) {
      img.src = 'textures/cubeTexture1.png';
   } else if(img2) {
      img.src = 'textures/cubeTexture2.png';
   } else if(img3) {
      img.src = 'textures/cubeTexture3.png';
   }

   img.webglTexture = false;

   img.onload = function(e) {
      var texture = gl_ctx.createTexture();

      gl_ctx.pixelStorei(gl_ctx.UNPACK_FLIP_Y_WEBGL, true);
      gl_ctx.bindTexture(gl_ctx.TEXTURE_2D, texture);
      gl_ctx.texParameteri(gl_ctx.TEXTURE_2D, gl_ctx.TEXTURE_MIN_FILTER, gl_ctx.LINEAR);
      gl_ctx.texParameteri(gl_ctx.TEXTURE_2D, gl_ctx.TEXTURE_MAG_FILTER, gl_ctx.LINEAR);

      gl_ctx.texImage2D(gl_ctx.TEXTURE_2D, 0, gl_ctx.RGBA, gl_ctx.RGBA, gl_ctx.UNSIGNED_BYTE, img);

      gl_ctx.bindTexture(gl_ctx.TEXTURE_2D, null);
      img.webglTexture = texture;
   };
   return img;

}



function gl_draw() {
   // set the color to transparent
   gl_ctx.clearColor(0.0, 0.0, 0.0, 0.0);
   // enable Depth buffer test and set depth buffer comparison function
   gl_ctx.enable(gl_ctx.DEPTH_TEST);
   gl_ctx.depthFunc(gl_ctx.LEQUAL);

   // set the clear value for the depth buffer to 1
   gl_ctx.clearDepth(1.0);

   var timeOld = 0;

   var animate = function (time) {
      var dAngle = rotationSpeed * (time - timeOld);

      gl_setMatrix;

      if (X) {
         MATRIX.rotateX(_matrixMovement, dAngle);
      }
      if (Y) {
         MATRIX.rotateY(_matrixMovement, dAngle);
      }
      if (Z) {
         MATRIX.rotateZ(_matrixMovement, dAngle);
      }

      timeOld = time;

      // set the drawing area on the canvas and clear it
      gl_ctx.viewport(0.0, 0.0, gl_canvas.width, gl_canvas.height);
      gl_ctx.clear(gl_ctx.COLOR_BUFFER_BIT | gl_ctx.DEPTH_BUFFER_BIT);

      // set projection matrix. _matrixProjection is not set yet.
      // It is a javascript array of 1 dimension with 16 floats
      gl_ctx.uniformMatrix4fv(_PosMatrix, false, _matrixProjection);
      gl_ctx.uniformMatrix4fv(_MovMatrix, false, _matrixMovement);
      gl_ctx.uniformMatrix4fv(_ViewMatrix, false, _matrixView);

      if (_cubeTexture.webglTexture) {
         gl_ctx.activeTexture(gl_ctx.TEXTURE0);
         gl_ctx.bindTexture(gl_ctx.TEXTURE_2D, _cubeTexture.webglTexture);
      }

      // drawing is here - use these points for next drawing
      gl_ctx.vertexAttribPointer(_position, 3, gl_ctx.FLOAT, false, 4*(3+2), 0);
      gl_ctx.vertexAttribPointer(_uv, 2, gl_ctx.FLOAT, false, 4*(3+2), 3*4);

      gl_ctx.bindBuffer(gl_ctx.ARRAY_BUFFER, _triangleVertexBuffer);
      gl_ctx.bindBuffer(gl_ctx.ELEMENT_ARRAY_BUFFER, _triangleFacesBuffer);

      // draw the triangle
      //gl_ctx.drawElements(gl_ctx.TRIANGLES, 3, gl_ctx.UNSIGNED_SHORT, 0);

      // draw cube
      gl_ctx.drawElements(gl_ctx.TRIANGLES, 5*2*3, gl_ctx.UNSIGNED_SHORT, 0);

      // drawing is finished - show the render
      gl_ctx.flush();
      // redraws the scene as soon as ready
      window.requestAnimationFrame(animate);
   };

   if(!running){
      // launch animate for the first time
      animate(0);
      running=true;
   }


}