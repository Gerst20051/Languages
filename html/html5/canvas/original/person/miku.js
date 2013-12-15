/* MIT-LICENSE */
/*
Copyright (c) 2009 Satoshi Ueyama

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* - about model data -  */
/* originally composed by Zusa */
/* converted by nakamura001 */
/* refer http://www32.atwiki.jp/nakamura001/pages/47.html */

function MikuApp()
{
	this.mTickCount = 0;
	this.vUP = new Vec3(0,  1, 0);
	this.vAT = new Vec3(0, 20, 0);
	this.mProjMat  = null;
	this.mWorldMat = null;
	this.mViewMat  = null;
	this.mAllMat   = null;

	this.mEyeA     = new Vec3(0, 0, 900);
	this.mEye      = new Vec3(0, 0, 1900);

	this.mViewport = {};
	this.mViewport.w = 480;
	this.mViewport.h = 300;
	this.mViewport.ow = 240;
	this.mViewport.oh = 150;

	this.mikuModel = new MikuModel(this);

	var _this = this;
	this.canvas = document.getElementById("cv");
	this.canvas.addEventListener("mousemove", function(e){_this.onMouseMove(e);}, false);
	this.canvas.addEventListener("mousedown", function(e){_this.onClick(e);}, false);

	P3D.g = this.canvas.getContext("2d");
	this.mSkyGrad = P3D.g.createLinearGradient(0,40, 0,this.mViewport.h);
	this.mSkyGrad.addColorStop(1, "#e0e5f3");
	this.mSkyGrad.addColorStop(0, "#c0d0e2");

	this.mLoadCount = 2;

	var tex = new Image();
	this.texture1 = tex;
	tex.onload = function(){ _this.start(); };
	tex.src = "http://img.f.hatena.ne.jp/images/fotolife/g/gyuque/20090227/20090227034206.png";

	tex = new Image();
	this.texture2 = tex;
	tex.onload = function(){ _this.start(); };
	tex.src = "http://img.f.hatena.ne.jp/images/fotolife/g/gyuque/20090227/20090227034205.png";
}

MikuApp.prototype = {
	start: function() {
		if (--this.mLoadCount > 0) return;

		this.setupTransform();

		this.onInterval();
	},

	onMouseMove: function(e) {
		this.setEyePosTarget(e.clientX - this.mViewport.ow, e.clientY - this.mViewport.oh);
	},

	setEyePosTarget: function(tx, ty) {
		this.mEyeA.z = Math.cos(tx*0.01) * 500 + 100;
		this.mEyeA.x = Math.sin(tx*0.01) * 500;
		this.mEyeA.y = 400 - ty*3.2;
	},

	onClick: function(e) {
		this.mikuModel.swing();
	},

	onInterval: function() {
		this.mikuModel.tick();
		this.mEye.smul(0.7);
		this.mEye.x += this.mEyeA.x * 0.3;
		this.mEye.y += this.mEyeA.y * 0.3;
		this.mEye.z += this.mEyeA.z * 0.3;

		P3D.clear("#fff", this.mViewport.w, this.mViewport.h);

		this.updateViewTrans();

		this.worldMatrix().ident();
		this.calcAllTransforms();

		this.drawSky();

		this.transformVertices(MESH_FLOOR);
		this.transformVertices(MESH_MAIN, this.mikuModel);

		P3D.texture = this.texture2;
		P3D.drawByIndexBuffer(MESH_FLOOR.tl_poss, MESH_FLOOR.sorted_indices, MESH_FLOOR.texcoords, 0);

		P3D.texture = this.texture1;
		P3D.drawByIndexBuffer(MESH_MAIN.tl_poss, MESH_MAIN.sorted_indices, MESH_MAIN.texcoords, -1);

		this.mTickCount++;
		var _this = this;
		setTimeout( function(){_this.onInterval();} , 20);
	},

	drawSky: function() {
		var mViewRot = new M44(this.mViewMat);
		mViewRot._41 = 0;
		mViewRot._42 = 0;
		mViewRot._43 = 0;

		mViewRot.transpose33();
		var spos = [0,0,0,0];
		
		mViewRot.transVec3(spos, 0, 0, 5000);
		this.mAllMat.transVec3(spos, spos[0], -100, spos[2]);

		var W = spos[3];
		spos[1] /= W;
		spos[1] *= -this.mViewport.h;
		spos[1] += this.mViewport.oh;

		if (spos[1] >= 0) {
			P3D.g.save();
			P3D.g.transform(1,0,0,1, 0, spos[1] - this.mViewport.h);
			P3D.g.beginPath();
			P3D.g.fillStyle = this.mSkyGrad;
			P3D.g.fillRect(0, 0, this.mViewport.w, this.mViewport.h);
			P3D.g.restore();
		}
	},

	calcAllTransforms: function() {
		if (!this._mTmpMat) this._mTmpMat = new M44();
		this._mTmpMat.mul(this.mWorldMat, this.mViewMat);
		this.mAllMat.mul(this._mTmpMat, this.mProjMat);
	},

	worldMatrix: function() {
		return this.mWorldMat;
	},

	setupTransform: function() {
		this.mWorldMat = new M44();

		this.mProjMat = new M44();
		this.mProjMat.perspectiveLH(24, 15, 16, 4000);

		this.mViewMat = new M44();
		this.updateViewTrans(0);
		this.mAllMat  = new M44();
	},

	updateViewTrans: function(ry) {
		this.mViewMat.lookAtLH(this.vUP, this.mEye, this.vAT);
	},

	transformVertices: function(mesh, model) {
		var m = this.mAllMat;

		var hw = this.mViewport.ow;
		var hh = this.mViewport.oh;
		var poss = mesh.poss;
		var indices = mesh.indices;

		if ((poss.length%3) != 0 || (indices.length%3) != 0)
			throw "invalid buffer length!";
		var len  = poss.length/3;
		var flen = indices.length/3;

		if (!mesh.tl_faces) {
			mesh.tl_faces = new Array(flen);
			mesh.sorted_indices = new Array(flen*3);
		}

		if (!mesh.tl_poss) // make transformed-lit buffer
			mesh.tl_poss = new Array(len*4);

		var tl_poss = mesh.tl_poss;

		var p = new Vec3();
		var pi = 0, ti = 0, spos = new Array(4);
		
		var gi = 0, grest;

		grest = mesh.groups[gi].vertices;
		if (model)
			model.enterGroupTransform(0);

		for (i = 0;i < len;i++) {
			p.x = poss[pi  ];
			p.y = poss[pi+1];
			p.z = poss[pi+2];

			m.transVec3(spos, p.x, p.y, p.z);

			var W = spos[3];
			spos[0] /= W;
			spos[1] /= W;
			spos[2] /= W;

			spos[0] *= this.mViewport.w;
			spos[1] *= -this.mViewport.h;
			spos[0] += hw;
			spos[1] += hh;

			tl_poss[ti    ] = spos[0];
			tl_poss[ti + 1] = spos[1];
			tl_poss[ti + 2] = spos[2];

			pi += 3;
			ti += 4;

			if (--grest == 0) {
				if (mesh.groups[++gi])
				{
					grest = mesh.groups[gi].vertices;

					if (model)
						model.enterGroupTransform(gi);
				}
			}
		}

		this.setupFaces(mesh, flen);
		mesh.tl_faces.sort(this.zsortCmp);

		var sorted_indices = mesh.sorted_indices;
		var tl_faces       = mesh.tl_faces;
		ti = 0;
		for (i = 0;i < flen;i++) {
			pi = (tl_faces[i]>>16) * 3;
			sorted_indices[ti++] = indices[pi++];
			sorted_indices[ti++] = indices[pi++];
			sorted_indices[ti++] = indices[pi  ];
		}
	},

	zsortCmp: function(a, b) {
		return (b&0xffff) - (a&0xffff);
	},

	setupFaces: function(mesh, flen) {
		var indices = mesh.indices;
		var faces   = mesh.tl_faces;
		var tl_poss = mesh.tl_poss;

		var i, ii = 0;
		var zk, gi = 0, grest, goffset = 0;
		grest = mesh.groups[gi].faces;
		for (i = 0;i < flen;i++) {
			if (!mesh.offset_applied) {
				indices[ii  ] += goffset;
				indices[ii+1] += goffset;
				indices[ii+2] += goffset;
			}

			zk  = tl_poss[ (indices[ii++] << 2) + 2];
			zk += tl_poss[ (indices[ii++] << 2) + 2];
			zk += tl_poss[ (indices[ii++] << 2) + 2];
			zk = Math.floor((zk-1.5)*10000);
			faces[i] = (i << 16) | zk;

			if (--grest == 0) {
				goffset += mesh.groups[gi].vertices;
				if (mesh.groups[++gi])
					grest = mesh.groups[gi].faces;
			}
		}
		mesh.offset_applied = true;
	}
}

function MikuModel(transformContext)
{
	this.mArmAngle = 100;
	this.mOmega = 0;
	this.mTransformContext = transformContext;
}

MikuModel.prototype = {
	tick: function() {
		this.mOmega += 0.5;
		if (this.mArmAngle < 60)
			this.mOmega += 1;
		if (this.mOmega > 8) this.mOmega = 8;

		this.mArmAngle += this.mOmega;
		if (this.mArmAngle < 10) this.mArmAngle = 10;
		if (this.mArmAngle > 100) this.mArmAngle = 100;
	},

	swing: function() {
		this.mOmega = -15;
	},

	enterGroupTransform: function(gi) {
		var w, t, u;
		switch(gi) {
		case 0:
			{
				w = this.mTransformContext.worldMatrix();
				w.ident();
				this.mTransformContext.calcAllTransforms();
			}
			break;

		case 1:
			{
				var aa = this.mArmAngle * 0.01745;

				var mx = 0, my = 22.0, mz = -0.5;

				t = new M44();
				u = new M44();
				w = this.mTransformContext.worldMatrix();


				t.translate(mx, -my, mz);
				u.glRotate(aa, 1.0, -0.2, 0.0);
				w.mul(t, u);
				t.copyFrom(w);

				u.translate(mx - 1, my - 7, mz - 1);
				w.mul(t, u);
				t.copyFrom(w);

				u.glRotate(0.1745, 0, 0, 1);
				w.mul(t, u);

				this.mTransformContext.calcAllTransforms();
			}
			break
		case 2:
			{
				w = this.mTransformContext.worldMatrix();
				t = new M44(w);
				u = new M44();

				u.glRotate(-1.9199, 1, 0, 0);
				w.mul(u, t);
				t.copyFrom(w);

				u.translate(30, 10, 25);
				w.mul(u, t);
				this.mTransformContext.calcAllTransforms();
			}
			break;
		}
	},

	leaveGroupTransform: function(gi) {
	}
}

var MESH_FLOOR = {
	groups: [
		{vertices: 4, faces: 8}
	],
	poss: [
		-120, -100, -120,
		  0 , -100, -120,
		-120, -100,  0,
		  0 , -100,  0,
		-120, -100, 120,
		  0 , -100, 120,

		 120, -100, -120,
		 120, -100, 0,
		 120, -100, 120
	],
	texcoords: [
		0,   0  ,
		0.5, 0  ,
		0,   0.5,
		0.5, 0.5,
		0,   1,
		0.5, 1,
		1, 0,
		1, 0.5,
		1, 1
	],
	indices: [
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
		1, 6, 3,
		3, 6, 7,
		3, 7, 5,
		5, 7, 8
	]
};