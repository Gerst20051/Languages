<?php
session_start();
chdir("/xampp/HomenetSpaces/hnsdesktop/");

include ("db.member.inc.php");
include ("login.inc.php");

if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { // user is logged in
include ("users_online.loggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
} else {
include ("users_online.notloggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
}
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
<title>Homenet Spaces OS | HTML5 Canvas!</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="A 3D chess game made with pure JavaScript, utilizing the Canvas element. 3D models for chess pieces are stored as JSON data.">
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<script type="text/javascript">
/*
3D Javascript Chess
Javascript/Canvas Textured 3D Renderer v0.3
Javascript/Canvas Textured 3D Renderer v0.3.1
Copyright (c) 2008 Jacob Seidelin, cupboy@gmail.com
This software is free to use for non-commercial purposes. For anything else, please contact the author.
*/

// -------------------------------------------------------------------------------
// XHR
// -------------------------------------------------------------------------------

// retrieves a file via XMLHTTPRequest, calls fncCallback when done or fncError on error.

var XHR = function(strURL, fncCallback, fncError) {
	var oHTTP = null;
	
	if (window.XMLHttpRequest) {
		oHTTP = new XMLHttpRequest();
	} else if (window.ActiveXObject) {
		oHTTP = new ActiveXObject("Microsoft.XMLHTTP");
	}
	
	if (oHTTP) {
		if (fncCallback) {
			if (typeof(oHTTP.onload) != "undefined")
				oHTTP.onload = function() {
					fncCallback(this);
					oHTTP = null;
				};
			else {
				oHTTP.onreadystatechange = function() {
					if (oHTTP.readyState == 4) {
						fncCallback(this);
						oHTTP = null;
					}
				};
			}
		}
		oHTTP.open("GET", strURL, true);
		oHTTP.setRequestHeader("If-Modified-Since", "Sat, 1 Jan 2000 00:00:00 GMT");
		oHTTP.send(null);
	} else {
		if (fncError) fncError();
	}
}

// -------------------------------------------------------------------------------
// Scene
// -------------------------------------------------------------------------------

var Canvas3D = {};

(function() {

Canvas3D.addEvent = function(oObject, strEvent, fncAction) {
	if (oObject.addEventListener) { 
		oObject.addEventListener(strEvent, fncAction, false); 
	} else if (oObject.attachEvent) { 
		oObject.attachEvent("on" + strEvent, fncAction); 
	}
}

Canvas3D.Scene = function(oContainer, iWidth, iHeight, bObjectCanvas) {

	this._oContainer = oContainer;

	this._iWidth = iWidth;
	this._iHeight = iHeight;

	var oCanvas = this._oDrawCanvas = document.createElement("canvas");
	this._oDrawContext = oCanvas.getContext("2d");
	Canvas3D.addEvent(oCanvas, "selectstart", function() {return false;});
	oCanvas.style.position = "absolute";

	this._oDrawCanvas.width = this._oDrawCanvas.style.width = iWidth;
	this._oDrawCanvas.height = this._oDrawCanvas.style.height = iHeight;

	oContainer.appendChild(this._oDrawCanvas);

	this._oActiveCamera = null;

	this._aObjects = [];

	this._bRunning = false;

	// if true, each object will be rendered on its own canvas
	this._bUseObjectCanvas = bObjectCanvas;

	this._bMouseRotate = true;
	this._bMouseRotateY = true;
	this._bMouseRotateX = true;

	this._oUpVector = new Canvas3D.Vec3(0,1,0);

	this._oAmbientLight = {r:50,g:50,b:50};

	this._bDrawLights = false;
	this._aLights = [];

	this._iMaxZ = 10000000;

	var oInputOverlay = this._oInputOverlay = document.createElement("span");
	oInputOverlay.style.width = this._iWidth+"px";
	oInputOverlay.style.height = this._iHeight+"px";
	oInputOverlay.style.zIndex = this._iMaxZ + 10000000;
	oInputOverlay.style.position = "absolute";
	this._oContainer.appendChild(oInputOverlay);

	Canvas3D.addEvent(oInputOverlay, "selectstart", function() {return false;});

}

Canvas3D.Scene.prototype.getInputLayer = function()
{
	return this._oInputOverlay;
}


Canvas3D.Scene.prototype.setUpVector = function(oVec)
{
	this._oUpVector = oVec;
}

Canvas3D.Scene.prototype.getUpVector = function()
{
	return this._oUpVector;
}

Canvas3D.Scene.prototype.getAmbientLight = function()
{
	return this._oAmbientLight;
}

Canvas3D.Scene.prototype.zoomCamera = function(fZoom)
{
	this.getActiveCamera().moveForward(fZoom);
}

Canvas3D.Scene.prototype.getObjects = function()
{
	return this._aObjects;
}

Canvas3D.Scene.prototype.addObject = function(obj)
{
	if (this._bUseObjectCanvas) {
		var oObjectCanvas = document.createElement("canvas");
		if (!oObjectCanvas.getContext) return;
	
		var oObjectContext = oObjectCanvas.getContext("2d");
	
		Canvas3D.addEvent(oObjectCanvas, "selectstart", function() {return false;});
	
		oObjectCanvas.width = this._iWidth;
		oObjectCanvas.height = this._iHeight;
	
		oObjectCanvas.style.width = this._iWidth+"px";
		oObjectCanvas.style.height = this._iHeight+"px";

		oObjectCanvas.style.position = "absolute";
		this._oContainer.appendChild(oObjectCanvas);

		this._aObjects.push(
			{
				canvas : oObjectCanvas,
				context : oObjectContext,
				object : obj
				
			}
		);

	} else {
		this._aObjects.push(
			{
				object : obj
			}
		);
	}

	obj.setScene(this);
	this.setDirty(true);
	return obj;
}

Canvas3D.Scene.prototype.removeObject = function(oObject)
{
	var aNewObjects = [];
	for (var i=0;i<this._aObjects.length;i++) {
		if (this._aObjects[i].object == oObject) {
			if (this._bUseObjectCanvas) {
				this._oContainer.removeChild(this._aObjects[i].canvas);
			}
		} else {
			aNewObjects.push(this._aObjects[i]);
		}
	}
	this._aObjects = aNewObjects;

}

Canvas3D.Scene.prototype.removeAllObjects = function()
{
	if (this._bUseObjectCanvas) {
		for (var i=0;i<this._aObjects.length;i++) {
			this._oContainer.removeChild(this._aObjects[i].canvas);
		}
	}
	this._aObjects = [];
}

Canvas3D.Scene.prototype.addLight = function(oLight)
{
	oLight.setScene(this);
	return this._aLights.push(oLight);
}

Canvas3D.Scene.prototype.getLights = function()
{
	return this._aLights;
}


Canvas3D.Scene.prototype.clearObjects = function()
{
	this._aObjects = [];
}


Canvas3D.Scene.prototype.setActiveCamera = function(oCam)
{
	this._oActiveCamera = oCam;
}

Canvas3D.Scene.prototype.getActiveCamera = function()
{
	return this._oActiveCamera;
}

Canvas3D.Scene.prototype.begin = function()
{
	this._bRunning = true;
	this.getActiveCamera().setDirty(true);
	this.drawAll();
	var me = this;
	this._iInterval = setInterval(function() { me.drawAll(); }, 1000 / 30);
}

Canvas3D.Scene.prototype.end = function()
{
	this._bRunning = false;
	clearInterval(this._iInterval);
}

Canvas3D.Scene.prototype.setDirty = function(bDirty)
{
	this._bDirty = bDirty;
}

Canvas3D.Scene.prototype.getDirty = function()
{
	return this._bDirty;
}

Canvas3D.Scene.prototype.getWidth = function()
{
	return this._iWidth;
}
Canvas3D.Scene.prototype.getHeight = function()
{
	return this._iHeight;
}


Canvas3D.Scene.prototype.drawAll = function()
{
	if (!this._bRunning) return;

	var oCam = this.getActiveCamera();

	var iOffsetX = Math.floor(this._iWidth / 2);
	var iOffsetY = Math.floor(this._iHeight / 2);

	var aObjects = this._aObjects;
	var bCamDirty = oCam.getDirty();
	var iObjDrawn = 0;

	var bObjDirty = false;
	for (var c=0;c<aObjects.length;c++) {
		if (aObjects[c].object.getDirty()) {
			bObjDirty = true;
		}
	}

	if (bCamDirty || this.getDirty() || bObjDirty) {
		if (bCamDirty || this.getDirty()) {
			this._oDrawContext.clearRect(0,0,this._iWidth,this._iHeight);
		}

		var aObjPos = [];
		for (var c=0;c<aObjects.length;c++) {
			if (aObjects[c].object.getSortPosition)
				var oPos = aObjects[c].object.getSortPosition();
			else
				var oPos = aObjects[c].object.getPosition();

			var oObjectPos = oCam.transformPoint(oPos);
			aObjPos[c] = [aObjects[c], oObjectPos.z];
		}

		var aSortObj = aObjPos.sort(
			function(a,b) {
				return b[1] - a[1];
			}
		);
		aObjects = aSortObj;

		for (var c=0;c<aObjects.length;c++) {
			var oObject = aObjects[c][0];
			if (oObject.object.isVisible()) {
				if (this._bUseObjectCanvas) {
					if (bCamDirty || this.getDirty() || oObject.object.getDirty()) {
						if (oObject.object.getForcedZ() > -1) {
							oObject.canvas.style.zIndex = oObject.object.getForcedZ();
						} else {
							var oObjectPos = oCam.transformPoint(oObject.object.getPosition());
							oObject.canvas.style.zIndex = 10000000 - Math.round(oObjectPos.z*100);
						}
						oObject.context.clearRect(0,0,this._iWidth,this._iHeight);
						if (oObject.object.draw(oObject.context, iOffsetX, iOffsetY)) {
							iObjDrawn++;
						}
						oObject.canvas.style.display = "block";
					}
				} else {
					if (bCamDirty || this.getDirty()) {
						if (oObject.object.draw(this._oDrawContext, iOffsetX, iOffsetY)) {
							iObjDrawn++;
						}
					}
				}
			} else {
				if (this._bUseObjectCanvas) {
					oObject.canvas.style.display = "none";
				}
			}
		}
	}

	if (this._bDrawLights) {
		for (var c=0;c<this._aLights.length;c++) {
			var oLight = this._aLights[c];
			oLight.draw(this._oLightContext, iOffsetX, iOffsetY);
		}
	}

	for (var c=0;c<this._aLights.length;c++) {
		this._aLights[c].setDirty(false);
	}
	oCam.setDirty(false);
	this.setDirty(false);

}

})();

// -------------------------------------------------------------------------------
// Vec3
// -------------------------------------------------------------------------------

(function() {

Canvas3D.Vec3 = function(vx, vy, vz) 
{
	this.x = vx;
	this.y = vy;
	this.z = vz;
}

Canvas3D.Vec3.prototype.set = function(vx, vy, vz) {
	this.x = vx;
	this.y = vy;
	this.z = vz;
}

Canvas3D.Vec3.prototype.addVector = function(V) {
	this.x += V.x;
	this.y += V.y;
	this.z += V.z;
	return this;
}


Canvas3D.Vec3.prototype.multiply = function(fScalar) {
	this.x *= fScalar;
	this.y *= fScalar;
	this.z *= fScalar;
	return this;
}


Canvas3D.Vec3.prototype.subVector = function(V) {
	this.x -= V.x;
	this.y -= V.y;
	this.z -= V.z;
	return this;
}

Canvas3D.Vec3.prototype.returnAdd = function(V) {
	return new Canvas3D.Vec3(this.x + V.x, this.y + V.y, this.z + V.z);
}

Canvas3D.Vec3.prototype.returnSub = function(V) {
	return new Canvas3D.Vec3(this.x - V.x, this.y - V.y, this.z - V.z);
}

Canvas3D.Vec3.prototype.clone = function() {
	return new Canvas3D.Vec3(this.x, this.y, this.z);
}

Canvas3D.Vec3.prototype.dot = function(V) {
	return ((this.x * V.x) + (this.y * V.y) + (this.z * V.z));
}

Canvas3D.Vec3.prototype.cross = function(V) {
	var vx = V.x;
	var vy = V.y;
	var vz = V.z;
	return new Canvas3D.Vec3((this.y * vz) - (this.z * vy), (this.z * vx) - (this.x * vz), (this.x * vy) - (this.y * vx));
}

Canvas3D.Vec3.prototype.length = function() {
	return Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
}

Canvas3D.Vec3.prototype.unit = function() {
	var l = 1/Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
	return new Canvas3D.Vec3(this.x * l, this.y * l, this.z * l);
}

Canvas3D.Vec3.prototype.rotateX = function(a) {
	var ry = this.y;
	var rz = this.z;
	var c = Math.cos(a);
	var s = Math.sin(a);
	this.y = c * ry - s * rz;
	this.z = s * ry + c * rz;
}

Canvas3D.Vec3.prototype.rotateY = function(a) {
	var rx = this.x;
	var rz = this.z;
	var c = Math.cos(a);
	var s = Math.sin(a);
	this.x = c * rx - s * rz;
	this.z = s * rx + c * rz;
}

Canvas3D.Vec3.prototype.rotateZ = function(a) {
	var rx = this.x;
	var ry = this.y;
	var c = Math.cos(a);
	var s = Math.sin(a);
	this.x = c * rx - s * ry;
	this.y = s * rx + c * ry;
}

Canvas3D.Vec3.prototype.dist = function(oVec)
{
	var x = oVec.x - this.x;
	var y = oVec.y - this.y;
	var z = oVec.z - this.z;
	return Math.sqrt(x*x + y*y + z*z);
}

})();

// -------------------------------------------------------------------------------
// Matrix3
// -------------------------------------------------------------------------------

Canvas3D.Matrix3 = function() 
{
	this.e = [
		[1.0, 0.0, 0.0],
		[0.0, 1.0, 0.0],
		[0.0, 0.0, 1.0]
	];
}

Canvas3D.Matrix3.prototype.multiplyVector = function(v)
{
	var p = new Canvas3D.Vec3;
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];

	var vx = v.x, vy = v.y, vz = v.z;

	p.x = vx * e0[0] + vy * e0[1] + vz * e0[2];
	p.y = vx * e1[0] + vy * e1[1] + vz * e1[2];
	p.z = vx * e2[0] + vy * e2[1] + vz * e2[2];
	return p;
}

Canvas3D.Matrix3.prototype.multiplyMatrix = function(M)
{
	var p = new Canvas3D.Matrix3();
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];

	var m0 = M.e[0], m1 = M.e[1], m2 = M.e[2];

	var e00 = e0[0], e01 = e0[1], e02 = e0[2];
	var e10 = e1[0], e11 = e1[1], e12 = e1[2];
	var e20 = e2[0], e21 = e2[1], e22 = e2[2];

	var m00 = m0[0], m01 = m0[1], m02 = m0[2];
	var m10 = m1[0], m11 = m1[1], m12 = m1[2];
	var m20 = m2[0], m21 = m2[1], m22 = m2[2];

	p.e[0][0] = m00 * e00 + m10 * e01 + m20 * e02;
	p.e[0][1] = m01 * e00 + m11 * e01 + m21 * e02;
	p.e[0][2] = m02 * e00 + m12 * e01 + m22 * e02;

	p.e[1][0] = m00 * e10 + m10 * e11 + m20 * e12;
	p.e[1][1] = m01 * e10 + m11 * e11 + m21 * e12;
	p.e[1][2] = m02 * e10 + m12 * e11 + m22 * e12;

	p.e[2][0] = m00 * e20 + m10 * e21 + m20 * e22;
	p.e[2][1] = m01 * e20 + m11 * e21 + m21 * e22;
	p.e[2][2] = m02 * e20 + m12 * e21 + m22 * e22;

	return p;
}

Canvas3D.Matrix3.prototype.transpose = function()
{
	var t = new Canvas3D.Matrix3();
	t.e[0][0] = this.e[0][0];
	t.e[0][1] = this.e[1][0];
	t.e[0][2] = this.e[2][0];
	t.e[1][0] = this.e[0][1];
	t.e[1][1] = this.e[1][1];
	t.e[1][2] = this.e[2][1];
	t.e[2][0] = this.e[0][2];
	t.e[2][1] = this.e[1][2];
	t.e[2][2] = this.e[2][2];
	return t;
}

Canvas3D.Matrix3.prototype.loadIdentity = function()
{
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];
	e0[0] = 1; e0[1] = 0; e0[2] = 0;
	e1[0] = 0; e1[1] = 1; e1[2] = 0;
	e2[0] = 0; e2[1] = 0; e2[2] = 1;
}

Canvas3D.Matrix3.prototype.loadRotationX = function(s, c)
{
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];
	e0[0] = 1; e0[1] = 0; e0[2] = 0;
	e1[0] = 0; e1[1] = c; e1[2] = -s;
	e2[0] = 0; e2[1] = s; e2[2] = c;
}

Canvas3D.Matrix3.prototype.loadRotationY = function(s, c)
{
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];
	e0[0] = c;  e0[1] = 0; e0[2] = s;
	e1[0] = 0;  e1[1] = 1; e1[2] = 0;
	e2[0] = -s; e2[1] = 0; e2[2] = c;
}

Canvas3D.Matrix3.prototype.loadRotationZ = function(s, c)
{
	var e0 = this.e[0], e1 = this.e[1], e2 = this.e[2];
	e0[0] = c; e0[1] = -s; e0[2] = 0;
	e1[0] = s; e1[1] = c;  e1[2] = 0;
	e2[0] = 0; e2[1] = 0;  e2[2] = 1;
}

Canvas3D.Matrix3.prototype.loadRotationAxis = function(A, s, c)
{
	var t = 1 - c;
	var tx = t * A.x;
	var ty = t * A.y;
	var txx = tx * A.x;
	var txy = tx * A.y;
	var txz = tx * A.z;
	var tyy = ty * A.y;
	var tyz = ty * A.z;
	var tzz = t * A.z * A.z;
	var sx = s * A.x;
	var sy = s * A.y;
	var sz = s * A.z;

	this.e[0][0] = txx + c;
	this.e[0][1] = txy - sz;
	this.e[0][2] = txz + sy;
	this.e[1][0] = txy + sz;
	this.e[1][1] = tyy + c;
	this.e[1][2] = tyz - sx;
	this.e[2][0] = txz - sy;
	this.e[2][1] = tyz + sx;
	this.e[2][2] = tzz + c;
}

// -------------------------------------------------------------------------------
// Camera
// -------------------------------------------------------------------------------

(function() {

Canvas3D.Camera = function() {

	this._oPosition = new Canvas3D.Vec3(0,0,0);

	this._oSideVec = new Canvas3D.Vec3(1,0,0);
	this._oUpVec = new Canvas3D.Vec3(0,1,0);
	this._oOutVec = new Canvas3D.Vec3(0,0,1);

	this._oRotMat = new Canvas3D.Matrix3();

	this._bDirty = false;
	this._fFocal = 500;
	this._fFocalDistance = this._fFocal;


	this._bReverseX = false;
	this._bReverseY = false;

	this._bTarget = true;

	this._iClipNear = 1;
	this._iClipFar = 10000000;

	this._fScale = 1;

	this._oLookAt = new Canvas3D.Vec3(0,0,0);

};

var proto = Canvas3D.Camera.prototype;

proto.getDirty = function()
{
	return this._bDirty;
}

proto.setDirty = function(bDirty)
{
	this._bDirty = bDirty;
}

proto.setPosition = function(oPos)
{
	this._oPosition.set(oPos.x, oPos.y, oPos.z);
	this._bDirty = true;
}

proto.getPosition = function()
{
	return this._oPosition;
}

proto.setScale = function(fScale)
{
	this._fScale = fScale;
	this._bDirty = true;
}

proto.getScale = function()
{
	return this._fScale;
}

proto.getSide = function()
{
	return this._oSideVec;
}

proto.getUp = function()
{
	return this._oUpVec;
}

proto.setUp = function(oVec)
{
	this._oUpVec = oVec;
}


proto.getOut = function()
{
	return this._oOutVec;
}


proto.moveSideways = function(d)
{
	this._oPosition.x += this._oSideVec.x * d;
	this._oPosition.y += this._oSideVec.y * d;
	this._oPosition.z += this._oSideVec.z * d;
	this.setDirty(true);
}

proto.moveUpwards = function(d)
{
	this._oPosition.x += this._oUpVec.x * d;
	this._oPosition.y += this._oUpVec.y * d;
	this._oPosition.z += this._oUpVec.z * d;
	this.setDirty(true);
}

proto.moveForward = function(d)
{
	this._oPosition.x += this._oOutVec.x * d;
	this._oPosition.y += this._oOutVec.y * d;
	this._oPosition.z += this._oOutVec.z * d;
	this.setDirty(true);
}

// rotate around the camera's side axis with a target center point (uses camera target if oTarget is null)
proto.pitchAroundTarget = function(fTheta, oTarget)
{
	var M = new Canvas3D.Matrix3();
	var oPos = this.getPosition();
	oTarget = oTarget || this.getLookAt();

	// translate position to target space
	oPos.subVector(oTarget);

	// rotate around side axis
	M.loadRotationAxis(this._oSideVec, Math.sin(fTheta * Math.PI / 180.0), Math.cos(fTheta * Math.PI / 180.0));
	oPos = M.multiplyVector(oPos);

	// translate position out of target space
	oPos.addVector(oTarget);

	this.setPosition(oPos);
	this.setDirty(true);
}

proto.yaw = function(fTheta)
{
	var M = new Canvas3D.Matrix3();
	M.loadRotationAxis(this._oUpVec, Math.sin(fTheta), Math.cos(fTheta));
	this._oSideVec = M.multiplyVector(this._oSideVec);
	this._oOutVec = M.multiplyVector(this._oOutVec);
	this.setDirty(true);
}

// rotate around the camera's up axis with a target center point (uses camera target if oTarget is null)
proto.yawAroundTarget = function(fTheta, oTarget)
{
	var M = new Canvas3D.Matrix3();
	var oPos = this.getPosition();
	oTarget = oTarget || this.getLookAt();

	// translate position to target space
	oPos.subVector(oTarget);

	// rotate around up axis
	M.loadRotationAxis(this._oUpVec, Math.sin(fTheta * Math.PI / 180.0), Math.cos(fTheta * Math.PI / 180.0));
	oPos = M.multiplyVector(oPos);

	// translate position out of target space
	oPos.addVector(oTarget);

	this.setPosition(oPos);
	this.setDirty(true);
}


// rotate around the camera's out axis with a target center point (uses camera target if oTarget is null)
proto.rollAroundTarget = function(fTheta, oTarget)
{
	var M = new Canvas3D.Matrix3();
	var oPos = this.getPosition();
	oTarget = oTarget || this.getLookAt();

	// translate position to target space
	oPos.subVector(oTarget);

	// rotate around out axis
	M.loadRotationAxis(this._oOutVec, Math.sin(fTheta * Math.PI / 180.0), Math.cos(fTheta * Math.PI / 180.0));
	oPos = M.multiplyVector(oPos);

	// translate position out of target space
	oPos.addVector(oTarget);

	this.setPosition(oPos);
	this.setDirty(true);
}

proto.rotateY = function(sine, cosine)
{
	var M = new Canvas3D.Matrix3();
	M.loadRotationY(sine, cosine);
	this._oSideVec = M.multiplyVector(this._oSideVec);
	this._oUpVec   = M.multiplyVector(this._oUpVec);
	this._oOutVec  = M.multiplyVector(this._oOutVec);
	this.setDirty(true);
}



proto.lookAt = function(P, Up)
{
	Up = Up || this._oUpVec;
	this._oOutVec  = P.returnSub(this._oPosition).unit();

	//this._oSideVec = this._oOutVec.cross(new Canvas3D.Vec3 (0.0, 1.0, 0.0)).unit();
	//this._oSideVec = this._oOutVec.cross(this._oUpVec).unit();
	this._oSideVec = this._oOutVec.cross(Up).unit();
	this._oUpVec   = this._oSideVec.cross(this._oOutVec).unit();
	this._vecLookAt = P.clone();
	this.setDirty(true);
}

proto.getLookAt = function() {
	return this._vecLookAt;
}

proto.updateRotationMatrix = function()
{
	var e0 = this._oRotMat.e[0];
	var e1 = this._oRotMat.e[1];
	var e2 = this._oRotMat.e[2];

	e0[0] = this._oSideVec.x;
	e0[1] = this._oSideVec.y;
	e0[2] = this._oSideVec.z;

	e1[0] = this._oUpVec.x;
	e1[1] = this._oUpVec.y;
	e1[2] = this._oUpVec.z;

	e2[0] = this._oOutVec.x;
	e2[1] = this._oOutVec.y;
	e2[2] = this._oOutVec.z;
}

proto.transformPoint = function(P)
{
	var e = this._oRotMat.e;
	var oPos = this._oPosition;

	var e0 = e[0];
	var e1 = e[1];
	var e2 = e[2];

	var vx = P.x - oPos.x;
	var vy = P.y - oPos.y;
	var vz = P.z - oPos.z;

	return new Canvas3D.Vec3(
		(vx * e0[0] + vy * e0[1] + vz * e0[2]),
		(vx * e1[0] + vy * e1[1] + vz * e1[2]),
		(vx * e2[0] + vy * e2[1] + vz * e2[2])
	);
}

proto.project = function(P)
{
	var fFocal = this._fFocal;
	return {
		x: P.x * fFocal / (P.z + this._fFocalDistance) * this._fScale * (this._bReverseX?-1:1),
		y: -P.y * fFocal / (P.z + this._fFocalDistance) * this._fScale * (this._bReverseY?-1:1)
	};
}

proto.clip = function(P) {
	if (P.z < 0) {
		return true;
	}
	return false;
}

proto.isBehind = function(P)
{
	if (P.z > 0) return false;
	return false;
}

proto.getClipNear = function()
{
	return this._iClipNear;
}

proto.getClipFar = function()
{
	return this._iClipFar;
}

proto.clip = function(P)
{
	if (P.z > this._iClipNear && P.z < this._iClipFar) {
		return false;
	} else {
		return true;
	}
}

proto.setFOV = function(fFOV)
{
	this._fFOV = fFOV;
	var fFocal = 1 / Math.tan(105 * Math.PI*Math.PI / (180*180 * 2));
	this._fFocal = fFocal;
	this._fFocalDistance = fFocal;
}

proto.getFOV = function()
{
	return this._fFOV;
}

proto.getFocal = function()
{
	return this._fFocal;
}

proto.setFocalDistance = function(fValue)
{
	this._fFocalDistance = fValue;
}

proto.setReverseX = function(bEnable)
{
	this._bReverseX = bEnable;
}

proto.setReverseY = function(bEnable)
{
	this._bReverseY = bEnable;
}

})();

// -------------------------------------------------------------------------------
// Mesh
// -------------------------------------------------------------------------------

Canvas3D.Mesh = function() {

	this._oPosition = new Canvas3D.Vec3(0,0,0);
	this._oRotation = new Canvas3D.Vec3(0,0,0);

	this._aVertices = []; // vertex positions in object space
	this._aGlobalVertices = []; // vertices translated to global space
	this._aFaces = [];
	this._aNormals = [];
	this._aMaterials = [];

	this._bDirty = true;

	this._bVisible = true;

	this._iForcedZ = -1;
	this._bHideWhenRotating = false;

	this._oDefaultColor = {r:155,g:155,b:155};
	this._oDefaultMaterial = {};
	this._iSize = 1;

	this._fScale = 1;

	this._bFill = true;		// render filled triangles
	this._bWire = false;		// render wireframe
	this._bShading = true;		// shade/light filled triangles
	this._bBackfaceCull = true;	// only draw triangles facing the camera
	this._bZSort = true;		// sort triangles back-to-front
	this._bExpandClipPath = true;	// expand clip path by 1px, to minimize gaps

	this._bTexture = false;		// render textured triangles (must enable bFill as well)
	this._bTextureShading = false;	// render shading on textured triangles (must enable bShading as well)

	// sometimes the exported normals from Max are messed up, or they're imported wrong or whatever, I don't know.
	// We can recalculate them after loading.
	this._bCalcNormals = true;

	// only allow textures if canvas is available
	var oCanvas = document.createElement("canvas");
	this._bCanTexture = false;	
	this._bCanTextureUV = false;	
	if (oCanvas.getContext && oCanvas.getContext("2d")) {
		this._bCanTexture = true;
		if (oCanvas.getContext("2d").getImageData) {
			this._bCanTextureUV = true;
		}
	}


};

// parse the mesh data
// the mesh data (vertices, faces, texture coordinates, materials) are read from a JSON object structure
// and copied into local arrays
// normals are recalculated, if enabled.
Canvas3D.Mesh.prototype.setMeshData = function(oMeshData, oScene) 
{
	this._oMeshData = oMeshData;

	this._aVertices = [];
	this._aFaces = [];
	this._aNormals = [];
	this._aMaterials = [];

	var oPos = this._oPosition;

	var me = this;

	if (this._oMeshData.mat) {
		for (var m=0;m<this._oMeshData.mat.length;m++) {
			var oMat = this._oMeshData.mat[m];
			oMat.idx = m;
			if (oMat.t) {
				oMat.image = new Image();
				if (oMat.uv && this._bCanTextureUV) {
					oMat.image.mat = oMat;
					oMat.image.onload = function() {
						me._bakeTexture(this.mat);
						if (oScene) {
							oScene.setDirty(true);
						}
						this.onload = null;
					}
				}
				oMat.image.src = "textures/" + oMat.t;
			}
			this._aMaterials.push(oMat);
		}
	}

	for (var o=0;o<this._oMeshData.obj.length;o++) {

		var oObject = this._oMeshData.obj[o];
		var aVertices = oObject.vrt;
		var aTexCoords = oObject.tex;

		this._aTexCoords = aTexCoords;

		var iVertOffset = this._aVertices.length;

		var fTotalX = 0;
		var fTotalY = 0;
		var fTotalZ = 0;

		var iNumVertices = aVertices.length;

		for (var v=0;v<iNumVertices;v++) {
			var oVertex = new Canvas3D.Vec3(
					aVertices[v][0],
					aVertices[v][1],
					aVertices[v][2]
			);

			this._aVertices.push(oVertex);

			this._aGlobalVertices.push(
				new Canvas3D.Vec3(
					oVertex.x + oPos.x,
					oVertex.y + oPos.y,
					oVertex.z + oPos.z
				)
			);

			fTotalX += oVertex.x;
			fTotalY += oVertex.y;
			fTotalZ += oVertex.z;
		}

		var fAvgX = fTotalX / iNumVertices;
		var fAvgY = fTotalY / iNumVertices;
		var fAvgZ = fTotalZ / iNumVertices;

		var oLocalCenter = new Canvas3D.Vec3(fAvgX, fAvgY, fAvgZ);

		var aFaces = oObject.fac;
		for (var f=0;f<aFaces.length;f++) {
			var oFace = aFaces[f];

			var oPoint1 = this._aGlobalVertices[oFace[0] + iVertOffset];
			var oPoint2 = this._aGlobalVertices[oFace[1] + iVertOffset];
			var oPoint3 = this._aGlobalVertices[oFace[2] + iVertOffset];

			var oCenter = new Canvas3D.Vec3(
				(oPoint1.x + oPoint2.x + oPoint3.x) / 3, 
				(oPoint1.y + oPoint2.y + oPoint3.y) / 3, 
				(oPoint1.z + oPoint2.z + oPoint3.z) / 3
			);

			var oNormal = new Canvas3D.Vec3(
					oObject.nrm[f][0],
					oObject.nrm[f][1],
					oObject.nrm[f][2]
			);

			var oFace = {
					a : oFace[0] + iVertOffset,
					b : oFace[1] + iVertOffset,
					c : oFace[2] + iVertOffset,
					normal : oNormal,
					center : oCenter,
					mat : oFace[3],
					idx : f,
					lights : []
			}

			this._aFaces.push(oFace);
		}

		if (this._bCalcNormals) {
			this._recalcNormals();
		}
	}
}

// here we prepare the texture for easy rendering. 
// For each face, a triangular region is drawn onto a canvas containing the image data for that face
// We draw a triangular section on the destination canvas containing the corresponding pixel data from the source texture,
// using the UV coords to retrieve the correct pixels
// that way, we have all texture parts rotated and scaled into a common form, so they can easily be drawn later
// at the moment, the data is read with getImageData and is rather slow, but it should be possible do to sort of the reverse process
// of the drawTextureTriangle transform to draw these texture parts.
Canvas3D.Mesh.prototype._bakeTexture = function(oMat) 
{

	var f = this._aFaces.length;

	var aTexFaces = [];
	// find all face that need this texture
	do {
		var oFace = this._aFaces[f-1];
		if (oFace.mat == oMat.idx) {
			aTexFaces.push(oFace);
			oFace.texidx = aTexFaces.length-1;
		}
	} while (--f)

	f = aTexFaces.length;
	if (f < 1) {
		return;
	}

	var fc = aTexFaces.length;

	var iTexRes = oMat.res;
	var iTexWidth = iTexRes * f + f*2;
	var iTexHeight = iTexRes + 2;

	var iSrcWidth = oMat.w;
	var iSrcHeight = oMat.h;

	// create canvas for source texture image and paint the texture on it
	var oSource = document.createElement("canvas");
	oSource.width = iSrcWidth;
	oSource.height = iSrcHeight;
	oSource.style.width = iSrcWidth+"px";
	oSource.style.height = iSrcHeight+"px";
	var oSrcCtx = oSource.getContext("2d");
	oSrcCtx.drawImage(oMat.image, 0, 0, iSrcWidth, iSrcHeight);
	var oSrcDataObj = oSrcCtx.getImageData(0, 0, iSrcWidth, iSrcHeight);
	var aSrcData = oSrcDataObj.data;

	// create canvas for finished face textures.
	var oTexCanvas = document.createElement("canvas");
	oTexCanvas.width = iTexWidth;
	oTexCanvas.height = iTexHeight;
	oTexCanvas.style.width = iTexWidth+"px";
	oTexCanvas.style.height = (iTexHeight)+"px";
	oTexCanvas.style.backgroundColor = "rgb(255,0,255)";
	oTexCanvas.resolution = iTexRes;
	var oDstCtx = oTexCanvas.getContext("2d");
	oDstCtx.fillStyle = "rgb(255,0,255)";
	oDstCtx.fillRect(0,0,iTexWidth,iTexHeight);

	var oDstDataObj = oDstCtx.getImageData(0, 0, iTexWidth, iTexHeight);
	var aDstData = oDstDataObj.data;

	var oContext = oTexCanvas.getContext("2d");

	//uncomment to see how the texture is prepared
	//document.body.appendChild(oSource);
	//document.body.appendChild(oTexCanvas);

	var iTexOffsetX = iTexRes;

	do {
		var oFace = aTexFaces[f-1];

		var oCoords = this._aTexCoords[oFace.idx];

		var oTexPoint1 = oCoords[1];
		var oTexPoint2 = oCoords[2];
		var oTexPoint3 = oCoords[0];

		var x1 = oTexPoint1[0] * iSrcWidth;
		var y1 = (1 - oTexPoint1[1]) * iSrcHeight;
		var x3 = oTexPoint2[0] * iSrcWidth;
		var y3 = (1 - oTexPoint2[1]) * iSrcHeight;
		var x2 = oTexPoint3[0] * iSrcWidth;
		var y2 = (1 - oTexPoint3[1]) * iSrcHeight;

		var fUnitAX = (x2 - x1);
		var fUnitAY = (y2 - y1);

		var fUnitBX = (x3 - x2);
		var fUnitBY = (y3 - y2);

		var iOffsetX = 0;

		var iDstXOffset = (iTexWidth - iTexOffsetX - (fc-f)*2-2);

		// we paint the triangular texture with a 1px margin on each side and let the texture spill over into this margin.
		// this is to prevent small transparent gaps to appear between the triangles when they are rotated and scaled into place during rendering.

		var y = iTexRes+2;
		do {
			var iDstY = iTexRes+2-y;
			var fStepY = (iTexRes+1-y) / iTexRes;

			var fStepYUnitBX = fStepY*fUnitBX;
			var fStepYUnitBY = fStepY*fUnitBY;

			var iDstYOffset = iDstY*iTexWidth*4;

			var x = iTexRes+2 - iOffsetX;
			do {
				var iDstX = x + iDstXOffset - 1;
				var fStepX = (x-1 + iOffsetX) / iTexRes;
	
				var iSrcX = Math.floor(x1 + fStepX*fUnitAX + fStepYUnitBX);
				var iSrcY = Math.floor(y1 + fStepX*fUnitAY + fStepYUnitBY);

				if (iSrcX < 0) iSrcX = 0;
				if (iSrcY < 0) iSrcY = 0;

				if (iSrcX >= iSrcWidth) iSrcX = iSrcWidth-1;
				if (iSrcY >= iSrcHeight) iSrcY = iSrcHeight-1;

				var iDstPixOffset = iDstYOffset + iDstX*4;
				var iSrcPixOffset = (iSrcY*iSrcWidth + iSrcX)*4;
	
				aDstData[iDstPixOffset] = aSrcData[iSrcPixOffset];
				aDstData[iDstPixOffset+1] = aSrcData[iSrcPixOffset+1];
				aDstData[iDstPixOffset+2] = aSrcData[iSrcPixOffset+2];

				aDstData[iDstPixOffset+3] = oMat.texalpha ? aSrcData[iSrcPixOffset+3] : 255;

			} while (--x);

			iOffsetX++;
			iDstXOffset++;

		} while (--y);

		iTexOffsetX += iTexRes;

	} while (--f);


	oDstCtx.putImageData(oDstDataObj, 0, 0);
	oDstCtx.fillRect(0,0,0,0); // Opera doesn't update until we draw something?
	oMat.facecanvas = oTexCanvas;
}

var fncZSort = function(a, b) { 
	return a.transcenter.z - b.transcenter.z;
}


// math and misc shortcuts
var sin = Math.sin;
var cos = Math.cos;
var asin = Math.asin;
var acos = Math.acos;
var pow = Math.pow;
var sqrt = Math.sqrt;

var fRadDeg = 180 / Math.PI;
var fDegRad = Math.PI / 180;
var fDegRad45 = fDegRad*45;
var fDegRad90 = fDegRad*90;
var fDegRad180 = fDegRad*180;
var fSqrt2Div2 = sqrt(2) / 2;


// this functions draws a textured (and shaded) triangle on the canvas
// this is done by rotating/scaling a triangular section in place, setting up a clipping path and drawing the image.
// if UV coords are enabled, only the correct part of the triangle-strip texture is drawn
// if not, the entire texture is drawn for each face
// some of the code used for skewing the image was inspired by the AS function found here:
// http://www.senocular.com/flash/actionscript.php?file=ActionScript_1.0/Prototypes/MovieClip/skew.as

Canvas3D.Mesh.prototype._drawTextureTriangle = function(oContext, oMat, oPoint1, oPoint2, oPoint3, iOffsetX, iOffsetY, fShade, oNormal, iIdx) 
{
	if (!oMat.image) {
		return;
	}
	if (!oMat.image.complete) {
		return;
	}

	var oMatImage = oMat.image;

	if (!oMatImage.canvas) {
		// first time around, we paint the texture image to a canvas
		// drawing the triangle later on is slightly faster using another canvas object rather than an image object
		// this should be moved to someplace else

		var iTexWidth = 50;
		var iTexHeight = 50;

		var oTextureCanvas = document.createElement("canvas");

		oTextureCanvas.width = iTexWidth;
		oTextureCanvas.height = iTexHeight;

		oTextureCanvas.style.width = iTexWidth + "px";
		oTextureCanvas.style.height = iTexHeight + "px";

		var oTexCtx = oTextureCanvas.getContext("2d");
		oTexCtx.drawImage(oMatImage, 0, 0, iTexWidth, iTexHeight);

		oMatImage.canvas = oTextureCanvas;
	}

	var x1 = oPoint1.x;
	var y1 = oPoint1.y;
	var x2 = oPoint2.x;
	var y2 = oPoint2.y;
	var x3 = oPoint3.x;
	var y3 = oPoint3.y;

	// trig to calc the angle we need to rotate in order get our texturetriangle in place
	var dx = x3 - x2;
	var dy = y3 - y2;
	var a = sqrt((dx*dx + dy*dy));

	dx = x3 - x1;
	dy = y3 - y1;
	var b = sqrt((dx*dx + dy*dy));

	dx = x2 - x1;
	dy = y2 - y1;
	var c = sqrt((dx*dx + dy*dy));

	var aa = a*a, bb = b*b, cc = c*c;

	var fCosB = (aa + cc - bb) / (2*a*c);
	var fAngleB = acos(fCosB);
	if (isNaN(fAngleB)) return;

	var fCosC = (aa + bb - cc) / (2*a*b);
	var fAngleC = acos(fCosC);
	if (isNaN(fAngleC)) return;

	if ((fAngleB + fAngleC) == 0) return;

	var fSkewX = -(fDegRad90 - (fAngleB + fAngleC));

	var fTriRotation = -(asin((y2 - y1) / c));

	if (x2 > x1) { // rotate the other way around if triangle is flipped
		fTriRotation = fDegRad180 - fTriRotation;
	}

	if (fSkewX == fDegRad90) fSkewX = fDegRad*89.99;
	if (fSkewX == -fDegRad90) fSkewX = -fDegRad*89.99;

	var fCosX = cos(fSkewX);

	var fRotation = fDegRad45 + fSkewX * 0.5;

	var fDiv = 1 / (sin(fRotation) * fSqrt2Div2);

	var fScaleX = fCosX * fDiv;
	var fScaleY = (sin(fSkewX) + 1) * fDiv;


	// setup the clipping path, so only texture within the triangle is drawn.
	var iClipX1 = x1 + iOffsetX;
	var iClipY1 = y1 + iOffsetY;

	var iClipX2 = x2 + iOffsetX;
	var iClipY2 = y2 + iOffsetY;

	var iClipX3 = x3 + iOffsetX;
	var iClipY3 = y3 + iOffsetY;


	// here we try to expand the clip path by 1 pixel to get rid of (some of the) gaps between the triangles
	// we do this simply by moving the topmost point 1px up, the leftmost point 1px left, and so on.
	// later, we also render the triangle itself 1 px too big
	// drawbacks are that the contour of the object will appear a bit "pointy".
	if (this._bExpandClipPath && false) {
		if (iClipY1 < iClipY2 && iClipY1 < iClipY3)
			iClipY1--;
		else if (iClipY2 < iClipY1 && iClipY2 < iClipY3)
			iClipY2--;
		else if (iClipY3 < iClipY1 && iClipY3 < iClipY2)
			iClipY3--;

		if (iClipY1 > iClipY2 && iClipY1 > iClipY3)
			iClipY1++;
		else if (iClipY2 > iClipY1 && iClipY2 > iClipY3)
			iClipY2++;
		else if (iClipY3 > iClipY1 && iClipY3 > iClipY2)
			iClipY3++;

		if (iClipX1 < iClipX2 && iClipX1 < iClipX3)
			iClipX1--;
		else if (iClipX2 < iClipX1 && iClipX2 < iClipX3)
			iClipX2--;
		else if (iClipX3 < iClipX1 && iClipX3 < iClipX2)
			iClipX3--;

		if (iClipX1 > iClipX2 && iClipX1 > iClipX3)
			iClipX1++;
		else if (iClipX2 > iClipX1 && iClipX2 > iClipX3)
			iClipX2++;
		else if (iClipX3 > iClipX1 && iClipX3 > iClipX2)
			iClipX3++;
	}

	oContext.save();

	// do the clip path
	oContext.beginPath();
	oContext.moveTo(iClipX1, iClipY1);
	oContext.lineTo(iClipX2, iClipY2);
	oContext.lineTo(iClipX3, iClipY3);
	oContext.closePath();
	oContext.clip();

	// setup the skew/rotation transformation
	oContext.translate(x2 + iOffsetX, y2 + iOffsetY);
	oContext.rotate(fRotation + fTriRotation);
	oContext.scale(fScaleX, fScaleY);
	oContext.rotate(-fDegRad45);

	var fTriScaleX = c / 2; // 100 * 50;
	var fTriScaleY = b / 2; // 100 * 50;

	if (oMat.uv) {
		// we are using UV coordinates for texturing
		if (this._bCanTextureUV && oMat.facecanvas) {
			var iTexRes = oMat.facecanvas.resolution;
			// draw our texture
			// there will be a small gap between the triangles. Drawing the texture at offset (-1,-1)  gets rid of some of it.
			oContext.drawImage(
				oMat.facecanvas, 
				iIdx * iTexRes + iIdx*2+1, 1, iTexRes, iTexRes,
				-1, -1, 
				fTriScaleX + 2,
				fTriScaleY + 2
			);
		}
	} else {
		// no UV, just draw the same texture for all faces
		oContext.drawImage(
			oMatImage.canvas, 
			-1, -1, 
			fTriScaleX + 2,
			fTriScaleY + 2
		);
	}


	oContext.restore();


	// if shading is turned on, render a semi-transparent black triangle on top.
	// that means that a fully lit triangle will just be the raw texture, and the less lit a triangle is, the darker it gets.
	// we could render semi-transparent white on top to make it brighter, but it doesn't look right, so we settle for that.
	if (this._bTextureShading && fShade > 0) {
		oContext.beginPath();
		oContext.moveTo(iClipX1, iClipY1);
		oContext.lineTo(iClipX2, iClipY2);
		oContext.lineTo(iClipX3, iClipY3);
		oContext.closePath();
	
		oContext.fillStyle = "rgba(0,0,0," + (fShade*0.5) + ")";
		oContext.fill();
	}
}

// draw the mesh on the oContext canvas context, at offset [iOffsetX, iOffsetY]
Canvas3D.Mesh.prototype.draw = function(oContext, iOffsetX, iOffsetY) 
{
	if (!this._bVisible) return;

	var oScene = this._oScene;

	var oCam = oScene.getActiveCamera();
	var oAmbient = oScene.getAmbientLight()

	// if shading is enabled, calculate the direction vectors to all light sources
	var bLightDirty = false;
	if (this._bShading && this._bFill) {
		var aLights = oScene.getLights();
		var aLightDirections = [];
		for (var l=0;l<aLights.length;l++) {
			// todo: this should be position relative to mesh
			var oLightPos = aLights[l].getPosition();
			var oLightDirection = oLightPos.unit();
			aLightDirections.push(oLightDirection);

			if (aLights[l].getDirty())
				bLightDirty = true;
		}
	}

	var aVertices = this._aGlobalVertices;
	var aFaces = this._aFaces;

	if (aVertices.length < 3 || aFaces.length < 1) {
		// nothing to draw
		return;
	}


	// let the camera transform all vertices and project them to 2D.
	var aPoints2D = [];
	var aTransVertices = [];
	var v = aVertices.length;
	do {
		var oVertex = aVertices[v-1];

		var oVec = oCam.transformPoint(oVertex);
		aTransVertices[v-1] = oVec;

		var oPoint2D = oCam.project(oVec);
		aPoints2D[v-1] = oPoint2D;
	} while (--v);

	var aSortedFaces;

	// if the faces are filled, we need to do z-sorting
	if (this._bFill && this._bZSort) {
		var f = aFaces.length;
		do {
			var oFace = aFaces[f-1];
			oFace.transcenter = oCam.transformPoint(oFace.center);
		} while (--f);

		aSortedFaces = aFaces.sort(fncZSort);

	// if not, just use the raw list
	} else {

		aSortedFaces = aFaces;
	}


	f = aSortedFaces.length;
	if (f < 1) {
		return;
	}

	// run through all faces
	do {
		var oFace = aSortedFaces[f-1];

		var oPoint1 = aPoints2D[oFace.a];
		var oPoint2 = aPoints2D[oFace.b];
		var oPoint3 = aPoints2D[oFace.c];

		var oNormal = oFace.normal;

		var bDraw = false;

		// do backface culling in screen space
		if (this._bBackfaceCull) {
			// screen space backface culling adapted from http://www.kirupa.com/developer/actionscript/backface_culling.htm
			if (((oPoint3.y-oPoint1.y)/(oPoint3.x-oPoint1.x) - (oPoint2.y-oPoint1.y)/(oPoint2.x-oPoint1.x) <= 0) ^ (oPoint1.x <= oPoint3.x == oPoint1.x > oPoint2.x)){
     				bDraw = true;
			}
		} else {
			bDraw = true;
		}

		if (oCam.clip(aTransVertices[oFace.a]) || oCam.clip(aTransVertices[oFace.b]) || oCam.clip(aTransVertices[oFace.c])) {
			bDraw = false;
		}

		
		// if triangle is facing camera, draw it
		if (bDraw) {

			// get the material for this face
			var oFaceMat = this._aMaterials[oFace.mat];
			if (oFaceMat) {
				oFaceColor = oFaceMat;
			} else {
				oFaceMat = this._oDefaultMaterial;
				oFaceColor = this._oDefaultColor;
			}
			var bFaceTexture = this._bTexture && oFaceMat.t;

			// save the original color
			var oFaceOrgColor = {r:oFaceColor.r, g:oFaceColor.g, b:oFaceColor.b};

			var fLight = 0;
			var fShade = 1;

			if (this._bFill) {
				// setup ambient face color
				if (!bFaceTexture) {
					if (bLightDirty || this._bDirty) {
						var oFaceColorAmb = {
							r:(oAmbient.r / 255) * oFaceColor.r,
							g:(oAmbient.g / 255) * oFaceColor.g,
							b:(oAmbient.b / 255) * oFaceColor.b
						};
					}
				}

				// do lighting
				if (this._bShading) {
					if (bLightDirty || this._bDirty) {
						for (var l=0;l<aLights.length;l++) {

							var oLightPos = aLights[l].getPosition();
							var oLightDir = new Canvas3D.Vec3(
								oLightPos.x - oFace.center.x,
								oLightPos.y - oFace.center.y,
								oLightPos.z - oFace.center.z
							).unit();

							var fDot = -oLightDir.dot(oNormal);

							// is the face facing the light source
							if (fDot > 0) {
								//fDot = Math.sqrt(fDot);
								fLight = fDot * aLights[l].getIntensity();
								fShade = fShade - fLight;

								// lighten the face by the light intensity
								if (!bFaceTexture) {
									oFaceColorAmb = {
										r: oFaceColorAmb.r + oFaceColor.r * fLight,
										g: oFaceColorAmb.g + oFaceColor.g * fLight,
										b: oFaceColorAmb.b + oFaceColor.b * fLight
									};
								}
							}
						}

						oFaceColorAmb.r = Math.floor(oFaceColorAmb.r);
						oFaceColorAmb.g = Math.floor(oFaceColorAmb.g);
						oFaceColorAmb.b = Math.floor(oFaceColorAmb.b);

						if (oFaceColorAmb.r < 0) oFaceColorAmb.r = 0;
						if (oFaceColorAmb.g < 0) oFaceColorAmb.g = 0;
						if (oFaceColorAmb.b < 0) oFaceColorAmb.b = 0;

						if (oFaceColorAmb.r > 255) oFaceColorAmb.r = 255;
						if (oFaceColorAmb.g > 255) oFaceColorAmb.g = 255;
						if (oFaceColorAmb.b > 255) oFaceColorAmb.b = 255;

						oFace.calccolor = oFaceColorAmb;
						oFace.shade = fShade;
					}
					oFaceColorAmb = oFace.calccolor;
					fShade = oFace.shade;
				}
				if (!bFaceTexture) {
					oFaceColor = oFaceColorAmb;
				}
			}

			oContext.beginPath();
			oContext.moveTo(oPoint1.x + iOffsetX, oPoint1.y + iOffsetY);
			oContext.lineTo(oPoint2.x + iOffsetX, oPoint2.y + iOffsetY)
			oContext.lineTo(oPoint3.x + iOffsetX, oPoint3.y + iOffsetY)
			oContext.closePath();

			if (this._bFill) {
				if (this._bCanTexture && this._bTexture && oFaceMat.image) {
					this._drawTextureTriangle(oContext, oFaceMat, oPoint1, oPoint2, oPoint3, iOffsetX, iOffsetY, fShade, oNormal, oFace.texidx);
				} else {
					oContext.fillStyle = "rgb(" + oFaceColor.r + "," + oFaceColor.g + "," + oFaceColor.b + ")";
					oContext.fill();
					if (!this._bWire) {
						oContext.lineWidth = 0.7;
						oContext.strokeStyle = "rgb(" + oFaceColor.r + "," + oFaceColor.g + "," + oFaceColor.b + ")";
						oContext.stroke();
					}
				}
			}

			if (this._bWire) {
				oFaceOrgColor.r = Math.min(Math.max(Math.round(oFaceOrgColor.r),0),255);
				oFaceOrgColor.g = Math.min(Math.max(Math.round(oFaceOrgColor.g),0),255);
				oFaceOrgColor.b = Math.min(Math.max(Math.round(oFaceOrgColor.b),0),255);

				oContext.lineWidth = 1;
				oContext.strokeStyle = "rgb(" + oFaceOrgColor.r + "," + oFaceOrgColor.g + "," + oFaceOrgColor.b + ")";
				oContext.stroke();
			}

		}
	} while (--f);
	this._bDirty = false;
}

Canvas3D.Mesh.prototype.setScene = function(oScene)
{
	if (this._oScene != oScene) {
		this._oScene = oScene;
	}
}

Canvas3D.Mesh.prototype.setLighting = function(bEnable)
{
	this._bShading = bEnable;
}

Canvas3D.Mesh.prototype.setBackfaceCull = function(bEnable)
{
	this._bBackfaceCull = bEnable;
}

Canvas3D.Mesh.prototype.setZSort = function(bEnable)
{
	this._bZSort = bEnable;
}

Canvas3D.Mesh.prototype.setFill = function(bEnable)
{
	this._bFill = bEnable;
}

Canvas3D.Mesh.prototype.setWire = function(bEnable)
{
	this._bWire = bEnable;
}

Canvas3D.Mesh.prototype.setTexture = function(bEnable)
{
	this._bTexture = bEnable;
}

Canvas3D.Mesh.prototype.setTextureShading = function(bEnable)
{
	this._bTextureShading = bEnable;
}

Canvas3D.Mesh.prototype._updateGlobalVertices = function()
{
	var oRot = this._oRotation;
	var oPos = this._oPosition;

	for (var i=0;i<this._aVertices.length;i++) {
		var oRotatedVertex = new Canvas3D.Vec3(
			this._aVertices[i].x,
			this._aVertices[i].y,
			this._aVertices[i].z
		);

		if (oRot.x)
			oRotatedVertex.rotateX(oRot.x);
		if (oRot.y)
			oRotatedVertex.rotateY(oRot.y);
		if (oRot.z)
			oRotatedVertex.rotateZ(oRot.z);

		this._aGlobalVertices[i].x = oRotatedVertex.x * this._fScale + oPos.x;
		this._aGlobalVertices[i].y = oRotatedVertex.y * this._fScale + oPos.y;
		this._aGlobalVertices[i].z = oRotatedVertex.z * this._fScale + oPos.z;
	}

	this._recalcNormals();
}

Canvas3D.Mesh.prototype._recalcNormals = function()
{
	for (var f=0;f<this._aFaces.length;f++) {
		var oFace = this._aFaces[f];

		var oPoint1 = this._aGlobalVertices[oFace.a];
		var oPoint2 = this._aGlobalVertices[oFace.b];
		var oPoint3 = this._aGlobalVertices[oFace.c];

		var oCenter = new Canvas3D.Vec3(
			(oPoint1.x + oPoint2.x + oPoint3.x) / 3, 
			(oPoint1.y + oPoint2.y + oPoint3.y) / 3, 
			(oPoint1.z + oPoint2.z + oPoint3.z) / 3
		);

		oFace.center = oCenter;

		var oNormal = new Canvas3D.Vec3(
			((oPoint1.y - oPoint2.y) * (oPoint1.z - oPoint3.z)) - ((oPoint1.z - oPoint2.z) * (oPoint1.y - oPoint3.y)),
			((oPoint1.z - oPoint2.z) * (oPoint1.x - oPoint3.x)) - ((oPoint1.x - oPoint2.x) * (oPoint1.z - oPoint3.z)),
			((oPoint1.x - oPoint2.x) * (oPoint1.y - oPoint3.y)) - ((oPoint1.y - oPoint2.y) * (oPoint1.x - oPoint3.x))
		).unit();
		oFace.normal = oNormal;
	}
}


Canvas3D.Mesh.prototype.setPosition = function(oVec)
{
	if (oVec.x != this._oPosition.x || oVec.y != this._oPosition.y || oVec.z != this._oPosition.z) {
		this._oPosition = oVec;
		this._updateGlobalVertices();
		this._bDirty = true;
	}
}

Canvas3D.Mesh.prototype.setRotation = function(oVec)
{
	this._oRotation = oVec;
	this._updateGlobalVertices();

	this._bDirty = true;

}

Canvas3D.Mesh.prototype.getPosition = function(oVec)
{
	return this._oPosition;
}

Canvas3D.Mesh.prototype.setForcedZ = function(iZ)
{
	this._iForcedZ = iZ;
}

Canvas3D.Mesh.prototype.getForcedZ = function()
{
	return this._iForcedZ;
}

Canvas3D.Mesh.prototype.getHideWhenRotating = function()
{
	return this._bHideWhenRotating;
}

Canvas3D.Mesh.prototype.setHideWhenRotating = function(bEnable)
{
	this._bHideWhenRotating = bEnable;
}

Canvas3D.Mesh.prototype.getDirty = function()
{
	return this._bDirty;
}


Canvas3D.Mesh.prototype.hide = function()
{
	this._bVisible = false;
	this._bDirty = true;
}

Canvas3D.Mesh.prototype.show = function()
{
	this._bVisible = true;
	this._bDirty = true;
}

Canvas3D.Mesh.prototype.isVisible = function()
{
	return this._bVisible;
}


Canvas3D.Mesh.prototype.setScale = function(fScale)
{
	this._fScale = fScale;
	this._bDirty = true;
	this._updateGlobalVertices();
}

Canvas3D.Mesh.prototype.getScale = function()
{
	return this._fScale;
}

// -------------------------------------------------------------------------------
// Light
// -------------------------------------------------------------------------------

(function() {

var __iLightIDCounter = 0;

Canvas3D.Light = function() {
	this._oPosition = new Canvas3D.Vec3(0,0,0);
	this._oColor = {r:255,g:255,b:255};
	this._fIntensity = 1.0;

	this._iLightID = ++__iLightIDCounter;
};

var proto = Canvas3D.Light.prototype;
	
proto.setPosition = function(oPos) 
{
	this._oPosition = oPos;
	this._bDirty = true;
}

proto.getPosition = function() 
{
	return this._oPosition;
}

proto.setColor = function(oColor) 
{
	this._oColor = oColor;
	this._bDirty = true;
}

proto.getColor = function() 
{
	return this._oColor;
}

proto.setIntensity = function(fIntensity) 
{
	this._fIntensity = fIntensity;
	this._bDirty = true;
}

proto.getIntensity = function(fIntensity) 
{
	return this._fIntensity;
}

proto.setScene = function(oScene)
{
	if (this._oScene != oScene) {
		this._oScene = oScene;
		this._bDirty = true;
	}
}

proto.draw = function(oContext, iOffsetX, iOffsetY) 
{
	var oScene = this._oScene;
	var oCam = oScene.getActiveCamera();

	var oPos2D = oCam.transform2D(oCam.transformPoint(this._oPosition));

	var iRadius = 3;

	oContext.beginPath();
	oContext.moveTo(oPos2D.x + iOffsetX + iRadius, oPos2D.y + iOffsetY);
	oContext.arc(oPos2D.x + iOffsetX, oPos2D.y + iOffsetY, iRadius, 0, 360, false);
	oContext.fillStyle = "rgb(255,255,0)";
	oContext.fill();
}

proto.getDirty = function() {
	return this._bDirty;
}

proto.setDirty = function(bDirty) {
	this._bDirty = bDirty;
}

})();

// -------------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------------

(function(){

var $ = function(id) {return document.getElementById(id);};

var fDegRad = Math.PI / 180;

var oScene;
var aPieces = [];


var bBoardLoaded = false;
var bKingLoaded = false;
var bQueenLoaded = false;
var bRookLoaded = false;
var bBishopLoaded = false;
var bKnightLoaded = false;
var bPawnLoaded = false;

var oSelectorCanvas;	// for drawing the selector square
var oValidCanvas;	// for drawing "valid move" squares

var iWidth = 700;
var iHeight = 500;

var oCam;

var iSelectorX = 0;
var iSelectorY = 0;
var bPieceSelected;
var oSelectedPiece;

var aMouseRegions;
var iLastMouseRegionX = -1;
var iLastMouseRegionY = -1;

var strActivePlayer = "white";

function getElementPos(oElement) {
	var iX = -(document.body.scrollLeft+document.documentElement.scrollLeft), iY = -(document.body.scrollTop+document.documentElement.scrollTop);
	while (oElement && oElement.nodeName != "BODY") {
		iX += oElement.offsetLeft;
		iY += oElement.offsetTop;
		oElement = oElement.offsetParent;
	}
	return {
		x : iX,
		y : iY
	}
}


function init() 
{

	oScene = new Canvas3D.Scene($("scenecontainer"), iWidth, iHeight, true);

	oCam = new Canvas3D.Camera();

	oCam.setPosition(new Canvas3D.Vec3(0,50,-100));

	oCam.setScale(18);
	oCam.setFOV(110);
	oCam.setFocalDistance(50);

	oCam.setReverseX(true);

	//oScene.setUpVector(new Canvas3D.Vec3(0,-1,0));

	oCam.lookAt(new Canvas3D.Vec3(0,0,0), oScene.getUpVector());

	oCam.updateRotationMatrix();

	oScene.setActiveCamera(oCam);

	oLight1 = new Canvas3D.Light();
	oLight1.setPosition(new Canvas3D.Vec3(0,30,0));
	oLight1.setIntensity(0.8);
	oScene.addLight(oLight1);

	oLight2 = new Canvas3D.Light();
	oLight2.setPosition(new Canvas3D.Vec3(100,30,100));
	oLight2.setIntensity(0.3);
	oScene.addLight(oLight2);

	oLight3 = new Canvas3D.Light();
	oLight3.setPosition(new Canvas3D.Vec3(-100,30,-100));
	oLight3.setIntensity(0.3);
	oScene.addLight(oLight3);

	XHR("board.json3d", onBoardLoaded);

	XHR("king.json3d", onKingLoaded);
	XHR("queen.json3d", onQueenLoaded);
	XHR("bishop.json3d", onBishopLoaded);
	XHR("knight.json3d", onKnightLoaded);
	XHR("rook.json3d", onRookLoaded);
	XHR("pawn.json3d", onPawnLoaded);

	Canvas3D.addEvent(document, "keydown",
		function(e) {
			e = e||window.event;
			var iKeyCode = e.keyCode;
			switch (iKeyCode) {
				case 102: // numpad 4
					moveSelectorLeft(); break;
				case 100: // numpad 6
					moveSelectorRight(); break;
				case 104: // numpad 8
					moveSelectorUp(); break;
				case 98: // numpad 2
					moveSelectorDown(); break;
				case 17: // ctrl
				case 101: // numpad 5
					if (bPieceSelected)
						attemptMovePiece();
					else 
						selectPiece();
			}
		}
	);

	oSelectorCanvas = document.createElement("canvas");
	oSelectorCanvas.width = iWidth;
	oSelectorCanvas.height = iHeight;
	oSelectorCanvas.style.width = iWidth+"px";
	oSelectorCanvas.style.height = iHeight+"px";
	oSelectorCanvas.style.zIndex = 1;
	oSelectorCanvas.style.position = "absolute";
	$("scenecontainer").appendChild(oSelectorCanvas);

	oValidCanvas = document.createElement("canvas");
	oValidCanvas.width = iWidth;
	oValidCanvas.height = iHeight;
	oValidCanvas.style.width = iWidth+"px";
	oValidCanvas.style.height = iHeight+"px";
	oValidCanvas.style.zIndex = 1;
	oValidCanvas.style.position = "absolute";
	$("scenecontainer").appendChild(oValidCanvas);


	var bIsRotating = false;
	var iMouseDownX = 0;
	var iMouseDownY = 0;
	var bMouseIsDown = false;
	var bMeshesHidden = false;

	Canvas3D.addEvent(oScene.getInputLayer(), "mousedown", 
		function(e) {
			e = e || window.event;
			iMouseDownX = e.clientX;
			iMouseDownY = e.clientY;
			bMouseIsDown = true;
		}
	);


	Canvas3D.addEvent(oScene.getInputLayer(), "click", 
		function(e) {
		}
	);

	var bUnhideOrdered = false;
	Canvas3D.addEvent(oScene.getInputLayer(), "DOMMouseScroll", 
		function(e) {
			e = e || window.event;

			if (!e.shiftKey) return;

			if (e.detail) {	
				e.wheelDelta = e.detail * -40;
			} 

			if (!bMeshesHidden) {
				for (var i=0;i<aPieces.length;i++) {
					aPieces[i].mesh.hide();
				}
				bMeshesHidden = true;
			}
			hideSelector();
			hideValidMoves();


			if (e.wheelDelta > 0) {
				oCam.setScale(oCam.getScale() * 1.5);
			} else {
				oCam.setScale(oCam.getScale() / 1.5);
			}

			if (!bUnhideOrdered) {
				setTimeout( function() {
					for (var i=0;i<aPieces.length;i++) {
						aPieces[i].mesh.show();
					}
					bMeshesHidden = false;
		
					bIsRotating = false;
					oScene.setDirty(true);

					updateMouseRegions();
					updateSelector();
					updateValidMoves();
		
					showSelector();
					showValidMoves();
					bUnhideOrdered = false;
				}, 100);
				bUnhideOrdered = true;
			}


			if (e.preventDefault)
				e.preventDefault();
			else
				e.returnValue = false;
		}
	);

	Canvas3D.addEvent(oScene.getInputLayer(), "mouseup", 
		function(e) {
			e = e || window.event;

			bMouseIsDown = false;
			if (bIsRotating) {
				for (var i=0;i<aPieces.length;i++) {
					aPieces[i].mesh.show();
				}
				bMeshesHidden = false;

				bIsRotating = false;
				oScene.setDirty(true);

				updateMouseRegions();
				updateSelector();
				updateValidMoves();

				setTimeout(showSelector, 10);
				setTimeout(showValidMoves, 10);
			} else {

				var oPos = getElementPos(oScene.getInputLayer());
				iMouseDownX = e.clientX - oPos.x;
				iMouseDownY = e.clientY - oPos.y;

				var oRegion = checkMouseRegions(e.clientX - oPos.x, e.clientY - oPos.y);
				if (oRegion) {
					iSelectorX = oRegion.x;
					iSelectorY = oRegion.y;
	
					updateSelector();
					if (bPieceSelected)
						attemptMovePiece();
					else 
						selectPiece();
				}
			}
		}
	);

	Canvas3D.addEvent(oScene.getInputLayer(), "mousemove",
		function(e) {
			e = e || window.event;
			if (bMouseIsDown) {
				if (!bMeshesHidden) {
					for (var i=0;i<aPieces.length;i++) {
						aPieces[i].mesh.hide();
					}
					bMeshesHidden = true;
				}


				hideSelector();
				hideValidMoves();

				bIsRotating = true;

				var iMouseX = e.clientX;
				var iMouseY = e.clientY;
				var fDeltaX = (iMouseX - iMouseDownX) / 3;
				var fDeltaY = -((iMouseY - iMouseDownY) / 3);

				// save the old camera position
				var oOldCamPos = new Canvas3D.Vec3(oCam.getPosition().x,oCam.getPosition().y,oCam.getPosition().z);

				// pitch the camera, but if we're not too low or if we're moving the camera up
				oCam.pitchAroundTarget(fDeltaY);
				if (!((oCam.getPosition().y > 15 || fDeltaY < 0) && (oCam.getPosition().y < 100 || fDeltaY > 0))) {
					oCam.setPosition(oOldCamPos);
				}

				oCam.yawAroundTarget(fDeltaX);
				oCam.lookAt(oCam.getLookAt(), oScene.getUpVector());
				oCam.updateRotationMatrix();
				iMouseDownX = e.clientX;
				iMouseDownY = e.clientY;

			} else {
				var oPos = getElementPos(oScene.getInputLayer());
				var oRegion = checkMouseRegions(e.clientX - oPos.x, e.clientY - oPos.y);
				if (oRegion) {
					var bNewRegion = false;
					if (iSelectorX != oRegion.x || iSelectorY != oRegion.y)
						bNewRegion = true;

					iSelectorX = oRegion.x;
					iSelectorY = oRegion.y;

					if (bNewRegion) { 
						updateSelector();
					}
				}
			}
		}
	);


	Canvas3D.addEvent(document, "keydown",
		function(e) {
			e = e || window.event;
			var iKeyCode = e.keyCode;
			var oCam;
			if (iKeyCode == 87) { // "w"
				if (oCam = oScene.getActiveCamera()) {
					var oTarget = oCam.getLookAt();
					var fDist = oTarget.dist(oCam.getPosition());
					var fMove = 50;
					if (fDist - fMove < 40) {
						fMove = fDist - 40;
					}
					oCam.moveForward(fMove);
					updateSelector();
				}
			}
			if (iKeyCode == 83) { // "s"
				if (oCam = oScene.getActiveCamera()) {
					oCam.moveForward(-50);
					updateSelector();
				}
			}
		}
	);


	oScene.begin();

	updateMouseRegions();
}


// setup polygons for board squares, used to check which piece mouse is hovering over
function updateMouseRegions() {
	var oCam = oScene.getActiveCamera();

	var iOffsetX = iWidth / 2;
	var iOffsetY = iHeight / 2;

	var aPaths = [];

	for (var x=0;x<8;x++) {
		aPaths[x] = [];

		for (var y=0;y<8;y++) {

			var iPosX = -(x-3) * 10;
			var iPosZ = (y-4) * 10;
			var iPosY = 0;

			var oP1 = oCam.project(oCam.transformPoint(new Canvas3D.Vec3(iPosX, iPosY, iPosZ)));
			var oP2 = oCam.project(oCam.transformPoint(new Canvas3D.Vec3(iPosX+10, iPosY, iPosZ)));
			var oP3 = oCam.project(oCam.transformPoint(new Canvas3D.Vec3(iPosX+10, iPosY, iPosZ+10)));
			var oP4 = oCam.project(oCam.transformPoint(new Canvas3D.Vec3(iPosX, iPosY, iPosZ+10)));

			aPaths[x][y] = [
				[oP1.x + iOffsetX, oP1.y + iOffsetY],
				[oP2.x + iOffsetX, oP2.y + iOffsetY],
				[oP3.x + iOffsetX, oP3.y + iOffsetY],
				[oP4.x + iOffsetX, oP4.y + iOffsetY]
			];
		}
	}
	aMouseRegions = aPaths;	
}

function checkMouseRegions(iMouseX, iMouseY) {
	var iRegionX = -1;
	var iRegionY = -1;

	var oCtx = oSelectorCanvas.getContext("2d");

	var bIsLast = false;
	if (iLastMouseRegionX > -1 && iLastMouseRegionY > -1) {
		var aLastRegion = aMouseRegions[iLastMouseRegionX][iLastMouseRegionY];
		if (checkSingleMouseRegion(oCtx, aLastRegion, iMouseX, iMouseY)) {
			bIsLast = true;
		}
	}
	var bFound = false;
	if (!bIsLast) {
		for (var x=0; x<8 && !bFound; x++) {
			for (var y=0; y<8 && !bFound; y++) {
				var aRegion = aMouseRegions[x][y];
				if (checkSingleMouseRegion(oCtx, aRegion, iMouseX, iMouseY)) {
					iRegionX = x;
					iRegionY = y;
					bFound = true;
				}
			}	
		}
	}

	if (iRegionX > -1 && iRegionY > -1) {
		return {
			x : iRegionX,
			y : iRegionY
		}
	}
}

function checkSingleMouseRegion(oCtx, aRegion, x, y) 
{
	oCtx.beginPath();
	oCtx.moveTo(aRegion[0][0], aRegion[0][1]);
	oCtx.lineTo(aRegion[1][0], aRegion[1][1]);
	oCtx.lineTo(aRegion[2][0], aRegion[2][1]);
	oCtx.lineTo(aRegion[3][0], aRegion[3][1]);
	oCtx.closePath();
	return oCtx.isPointInPath(x, y);
}

function moveSelectorLeft() {
	moveSelector(-1,0);
}
function moveSelectorRight() {
	moveSelector(1,0);
}
function moveSelectorUp() {
	moveSelector(0,1);
}
function moveSelectorDown() {
	moveSelector(0,-1);
}

function moveSelector(iDeltaX, iDeltaY) {
	var fAngle = Math.atan2(oCam.getPosition().x,oCam.getPosition().z) /  fDegRad;
	// four scenarios for how to move the selector, depending on camera angle
	if (fAngle >= 135 || fAngle <= -135) {
		iSelectorX += iDeltaX;
		iSelectorY += iDeltaY;
	} else if (fAngle >= -45 && fAngle <= 45) {
		iSelectorX -= iDeltaX;
		iSelectorY -= iDeltaY;
	} else if (fAngle >= -135 && fAngle <= -45) {
		iSelectorX -= iDeltaY;
		iSelectorY += iDeltaX;
	} else if (fAngle >= 45 && fAngle <= 135) {
		iSelectorX += iDeltaY;
		iSelectorY -= iDeltaX;
	}
 	if (iSelectorX < 0) iSelectorX = 7;
 	if (iSelectorX > 7) iSelectorX = 0;
 	if (iSelectorY > 7) iSelectorY = 0;
 	if (iSelectorY < 0) iSelectorY = 7;
	updateSelector();
}

function hideSelector() 
{
	oSelectorCanvas.style.display = "none";
}
function showSelector() 
{
	oSelectorCanvas.style.display = "block";
}

function hideValidMoves() 
{
	oValidCanvas.style.display = "none";
}
function showValidMoves() 
{
	oValidCanvas.style.display = "block";
}

function updateSelector() {
	var oCtx = oSelectorCanvas.getContext("2d");
	oCtx.clearRect(0,0,iWidth,iHeight);

	var iOffsetX = iWidth / 2;
	var iOffsetY = iHeight / 2;

	// draw active selector square

	makeRegionPath(oCtx, iSelectorX, iSelectorY);
	oCtx.fillStyle = "rgba(255,255,0,0.5)";
	oCtx.fill();

	if (bPieceSelected) {
		// draw border around selected square
		makeRegionPath(oCtx, oSelectedPiece.pos[0], oSelectedPiece.pos[1]);
		oCtx.lineWidth = 2;
		oCtx.strokeStyle = "rgba(255,0,0,1)";
		oCtx.stroke();
	}
}

function makeRegionPath(oCtx, x,y) 
{
	var aRegion = aMouseRegions[x][y];
	oCtx.beginPath();
	oCtx.moveTo(aRegion[0][0], aRegion[0][1]);
	oCtx.lineTo(aRegion[1][0], aRegion[1][1]);
	oCtx.lineTo(aRegion[2][0], aRegion[2][1]);
	oCtx.lineTo(aRegion[3][0], aRegion[3][1]);
	oCtx.closePath();
}

// select a piece
function selectPiece() 
{
	var iX = iSelectorX;
	var iY = iSelectorY;

	if (iX < 0 || iX > 7 || iY < 0 || iY > 7) {
		return false;
	}
	for (var i=0;i<aPieces.length;i++) {
		if (aPieces[i].pos[0] == iX && aPieces[i].pos[1] == iY) {
			oPiece = aPieces[i];
			if (oPiece.color == strActivePlayer) {
				oSelectedPiece = aPieces[i];
				bPieceSelected = true;
			}
		}
	}
	updateSelector();
	updateValidMoves();
}

function updateValidMoves() {
	var oCtx = oValidCanvas.getContext("2d");
	oCtx.clearRect(0,0,iWidth,iHeight);
	oCtx.fillStyle = "rgba(0,255,0,0.2)";

	if (!oSelectedPiece) return;

	for (var x=0; x<8; x++) {
		for (var y=0; y<8; y++) {
			if (isValidMove(oSelectedPiece, x, y)) {
				makeRegionPath(oCtx, x, y);
				oCtx.fill();
			}
		}	
	}
}

// attempt to move the selected piece
function attemptMovePiece() 
{
	var iX = iSelectorX;
	var iY = iSelectorY;

	if (oSelectedPiece.pos[0] == iX && oSelectedPiece.pos[1] == iY) {
		// cancel move
		bPieceSelected = false;
		oSelectedPiece = null;
		updateSelector();
		updateValidMoves();
		return;
	}

	var oTargetPiece = squareHasPiece(iX, iY);

	if (oTargetPiece) {
		if (oTargetPiece.color == strActivePlayer) {
			// select another piece
			bPieceSelected = false;
			oSelectedPiece = null;
			selectPiece();
			return;
		}
	}

	var bValidMove = isValidMove(oSelectedPiece, iX, iY);
	if (!bValidMove) return;

	var oTargetPiece = squareHasPiece(iX, iY);

	oSelectedPiece.pos[0] = iX;
	oSelectedPiece.pos[1] = iY;

	oSelectedPiece.hasmoved = true;

	oSelectedPiece = null;
	bPieceSelected = false;

	updateBoard();
	updateValidMoves();

	if (oTargetPiece) {
		removePiece(oTargetPiece);
		if (oTargetPiece.type == "king") {
			endGame();
		}
	}

	endMove();
}

function endGame() {
	alert("Congratulations, you have won!");
}

// removes a piece from the board (and the game)
function removePiece(oPiece) 
{
	var aNewPieces = [];
	for (var i=0;i<aPieces.length;i++) {
		if (aPieces[i] != oPiece) {
			aNewPieces.push(aPieces[i]);
		}
	}
	aPieces = aNewPieces;
	oScene.removeObject(oPiece.mesh);
}


// validates a proposed move. Returns true if the piece is allowed to make such a move.
function isValidMove(oPiece, iX, iY) 
{

	var iXDir = 1;
	if (oPiece.color == "black") {
		iXDir = -1;
	}

	if (oPiece.type ==  "pawn") { 
		if (iX*iXDir <= oPiece.pos[0]*iXDir) {
			return false;
		}
		if (iY == oPiece.pos[1]) {
			if (iX == oPiece.pos[0]+(1*iXDir)) {
				if (!squareHasPiece(iX, iY)) {
					return true;
				}
			} else if (iX == oPiece.pos[0]+(2*iXDir) && !oPiece.hasmoved) {
				if (!(squareHasPiece(iX, iY) || squareHasPiece(iX-(1*iXDir), iY))) {
					return true;
				}
			}
		} else if (iY == oPiece.pos[1]+1 || iY == oPiece.pos[1]-1) {
			if (iX == oPiece.pos[0]+(1*iXDir)) {
				var oTargetPiece;
				if (oTargetPiece = squareHasPiece(iX, iY)) {
					if (oTargetPiece.color != oPiece.color) {
						return true;
					}
				}
			}
		}
	}

	if (oPiece.type == "knight") {
		if (
			(iX == oPiece.pos[0]+2 && iY == oPiece.pos[1]-1) ||
			(iX == oPiece.pos[0]+2 && iY == oPiece.pos[1]+1) ||
			(iX == oPiece.pos[0]-2 && iY == oPiece.pos[1]-1) ||
			(iX == oPiece.pos[0]-2 && iY == oPiece.pos[1]+1) ||
			(iX == oPiece.pos[0]+1 && iY == oPiece.pos[1]-2) ||
			(iX == oPiece.pos[0]+1 && iY == oPiece.pos[1]+2) ||
			(iX == oPiece.pos[0]-1 && iY == oPiece.pos[1]-2) ||
			(iX == oPiece.pos[0]-1 && iY == oPiece.pos[1]+2)
		) {
			var oTargetPiece;
			if (oTargetPiece = squareHasPiece(iX, iY)) {
				if (oTargetPiece.color != oPiece.color) {
					return true;
				}
			} else {
				return true;
			}
		}
	}

	if (oPiece.type == "rook" || oPiece.type == "queen") {
		if (iX == oPiece.pos[0] || iY == oPiece.pos[1]) {
			if (iX == oPiece.pos[0]) {
				if (oPiece.pos[1] < iY) {
					for (var y=oPiece.pos[1]+1;y<iY;y++) {
						if (squareHasPiece(iX, y)) {
							return false;
						}
					}
				} else {
					for (var y=oPiece.pos[1]-1;y>iY;y--) {
						if (squareHasPiece(iX, y)) {
							return false;
						}
					}
				}
			}
			if (iY == oPiece.pos[1]) {
				if (oPiece.pos[0] < iX) {
					for (var x=oPiece.pos[0]+1;x<iX;x++) {
						if (squareHasPiece(x, iY)) {
							return false;
						}
					}
				} else {
					for (var x=oPiece.pos[0]-1;x>iX;x--) {
						if (squareHasPiece(x, iY)) {
							return false;
						}
					}
				}
			}
			var oTargetPiece;
			if (oTargetPiece = squareHasPiece(iX, iY)) {
				if (oTargetPiece.color == oPiece.color) {
					return false;
				}
			}
			return true;
		}
	}
	
	if (oPiece.type == "king") {
		if (
			(iX == oPiece.pos[0] && iY == oPiece.pos[1]+1) || 
			(iX == oPiece.pos[0] && iY == oPiece.pos[1]-1) || 
			(iX == oPiece.pos[0]+1 && iY == oPiece.pos[1]) || 
			(iX == oPiece.pos[0]-1 && iY == oPiece.pos[1]) || 
			(iX == oPiece.pos[0]+1 && iY == oPiece.pos[1]+1) || 
			(iX == oPiece.pos[0]-1 && iY == oPiece.pos[1]+1) || 
			(iX == oPiece.pos[0]+1 && iY == oPiece.pos[1]-1) || 
			(iX == oPiece.pos[0]-1 && iY == oPiece.pos[1]-1)
		) {
			var oTargetPiece;
			if (oTargetPiece = squareHasPiece(iX, iY)) {
				if (oTargetPiece.color == oPiece.color) {
					return false;
				}
			}
			return true;
		}
	}

	if (oPiece.type == "bishop" || oPiece.type == "queen") {
		// only diagonally
		if (Math.abs(iX - oPiece.pos[0]) == Math.abs(iY - oPiece.pos[1])) {
			if (iX > oPiece.pos[0]) {
				if (oPiece.pos[1] < iY) {
					var x = oPiece.pos[0]+1
					for (var y=oPiece.pos[1]+1;y<iY;y++) {
						if (squareHasPiece(x, y)) {
							return false;
						}
						x++;
					}
				} else {
					var x = oPiece.pos[0]+1
					for (var y=oPiece.pos[1]-1;y>iY;y--) {
						if (squareHasPiece(x, y)) {
							return false;
						}
						x++;
					}
				}
			}
			if (iX < oPiece.pos[0]) {
				if (oPiece.pos[1] < iY) {
					var x = oPiece.pos[0]-1
					for (var y=oPiece.pos[1]+1;y<iY;y++) {
						if (squareHasPiece(x, y)) {
							return false;
						}
						x--;
					}
				} else {
					var x = oPiece.pos[0]-1
					for (var y=oPiece.pos[1]-1;y>iY;y--) {
						if (squareHasPiece(x, y)) {
							return false;
						}
						x--;
					}
				}
			}
			var oTargetPiece;
			if (oTargetPiece = squareHasPiece(iX, iY)) {
				if (oTargetPiece.color == oPiece.color) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

function squareHasPiece(iX, iY) {
	for (var i=0;i<aPieces.length;i++) {
		if (aPieces[i].pos[0] == iX && aPieces[i].pos[1] == iY) {
			return aPieces[i];
		}
	}
}

function movePieceTo(oPiece, iX, iY)
{
	var oPos = getBoardPos(iX, iY);
	oPiece.mesh.setPosition(new Canvas3D.Vec3(oPos.x, 0, oPos.y));
}


function endMove() {
	updateSelector();
	if (strActivePlayer == "white") {
		strActivePlayer = "black";
	} else {
		strActivePlayer = "white";
	}
}


function createMesh(strText) {
	var oMeshData = eval("("+strText+")");
	if (oMeshData) {
		var oMesh = new Canvas3D.Mesh();

		oMesh._bShading = true;
		oMesh._bWire = false;
		oMesh._bFill = true;
		oMesh._bZSort = true;
		oMesh._bBackfaceCull = true;
		oMesh._bTexture = false;
		oMesh._bTextureShading = false;

		oMesh.setMeshData(oMeshData, oScene);
		return oMesh;
	}
}

function onBoardLoaded(oHTTP) 
{
	var oMesh = createMesh(oHTTP.responseText);
	oMesh.setForcedZ(0);
	oScene.addObject(oMesh);
	bBoardLoaded = true;
	setupGame();
}

function makePiece(strMeshText, strColor, fRot) {
	var oMesh = createMesh(strMeshText);
	for (var a=0;a<oMesh._aMaterials.length;a++) {
		if (strColor == "black") {
			oMesh._aMaterials[a].r = 100;
			oMesh._aMaterials[a].g = 80;
			oMesh._aMaterials[a].b = 80;
		} else if (strColor == "white") {
			oMesh._aMaterials[a].r = 220;
			oMesh._aMaterials[a].g = 220;
			oMesh._aMaterials[a].b = 220;
		}
	}
	if (fRot) {
		oMesh.setRotation(new Canvas3D.Vec3(0,fRot * fDegRad,0));
	}
	return oMesh;
}

function onKingLoaded(oHTTP) 
{
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white", 90)), 
		color : "white",
		type : "king",
		pos : [0,3]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black", -90)), 
		color : "black",
		type : "king",
		pos : [7,4]
	});
	bKingLoaded = true;
	setupGame();
}

function onQueenLoaded(oHTTP) 
{
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
		color : "white",
		type : "queen",
		pos : [0,4]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
		color : "black",
		type : "queen",
		pos : [7,3]
	});
	bQueenLoaded = true;
	setupGame();
}

function onBishopLoaded(oHTTP) 
{
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white", 180)), 
		color : "white",
		type : "bishop",
		pos : [0,5]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white", 180)), 
		color : "white",
		type : "bishop",
		pos : [0,2]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
		color : "black",
		type : "bishop",
		pos : [7,5]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
		color : "black",
		type : "bishop",
		pos : [7,2]
	});
	bBishopLoaded = true;
	setupGame();
}

function onKnightLoaded(oHTTP) 
{
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
		color : "white",
		type : "knight",
		pos : [0,6]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
		color : "white",
		type : "knight",
		pos : [0,1]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black", 180)), 
		color : "black",
		type : "knight",
		pos : [7,6]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black", 180)), 
		color : "black",
		type : "knight",
		pos : [7,1]
	});
	bKnightLoaded = true;
	setupGame();
}


function onRookLoaded(oHTTP) 
{
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
		color : "white",
		type : "rook",
		pos : [0,7]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
		color : "white",
		type : "rook",
		pos : [0,0]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
		color : "black",
		type : "rook",
		pos : [7,7]
	});
	aPieces.push({
		mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
		color : "black",
		type : "rook",
		pos : [7,0]
	});
	bRookLoaded = true;
	setupGame();
}

function onPawnLoaded(oHTTP) 
{
	for (var i=0;i<8;i++) {
		aPieces.push({
			mesh : oScene.addObject(makePiece(oHTTP.responseText, "white")), 
			color : "white",
			type : "pawn",
			pos : [1,i]
		});
		aPieces.push({
			mesh : oScene.addObject(makePiece(oHTTP.responseText, "black")), 
			color : "black",
			type : "pawn",
			pos : [6,i]
		});
	}
	bPawnLoaded = true;
	setupGame();
}


iBoardScale = 10;
function getBoardPos(x, y) {
	return {
		x : -((x - 4) * 10 + 5),
		y : (y - 4) * 10 + 5
	}
}

function setupGame() {
	if (!(bBoardLoaded && bKingLoaded && bQueenLoaded && bRookLoaded && bBishopLoaded && bPawnLoaded)) {
		return;
	}
	for (var i=0;i<aPieces.length;i++) {
		aPieces[i].hasmoved = false;
	}
	updateBoard();
	updateSelector();
}

function updateBoard() {
	for (var i=0;i<aPieces.length;i++) {
		var oPiece = aPieces[i];
		var oPos = getBoardPos(oPiece.pos[0], oPiece.pos[1])
		oPiece.mesh.setPosition(new Canvas3D.Vec3(oPos.x, 0, oPos.y));
	}
}
Canvas3D.addEvent(window, "load", init);

})();
</script>
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<div id="scenecontainer" style="background-color: #404d4f; border: 1px solid black; height: 500px; width: 700px;">
</div>
</div>
<!-- End page content -->
<?php
include ("tracking_scripts.inc.php");
?>
</body>

</html>
<?php
mysql_close($db);
?>