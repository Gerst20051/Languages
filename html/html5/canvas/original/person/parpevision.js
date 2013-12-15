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

window.P3D = {
	texture: null,
	g: null
};

P3D.clear = function(f, w, h) {
	var g = this.g;
	g.beginPath();
	g.fillStyle = f;
	g.fillRect(0, 0, w, h);

}

P3D.num_cmp = function(a,b){return a-b;}

P3D.drawTriangle = function(poss, uvs, shade_clr) {
	var w = this.texture.width;
	var h = this.texture.height;

	var g = this.g;
	
	var vAd = [ poss[1].x - poss[0].x , poss[1].y - poss[0].y ];
	var vBd = [ poss[2].x - poss[0].x , poss[2].y - poss[0].y ];

	var vA = [ uvs[1].u - uvs[0].u , uvs[1].v - uvs[0].v ];
	var vB = [ uvs[2].u - uvs[0].u , uvs[2].v - uvs[0].v ];

	vA[0] *= w;
	vA[1] *= h;

	vB[0] *= w;
	vB[1] *= h;

	var m = new M22();
	m._11 = vA[0];
	m._12 = vA[1];
	m._21 = vB[0];
	m._22 = vB[1];

	var im = m.getInvert();
	if (!im) return false;

	var a = im._11 * vAd[0] + im._12 * vBd[0];
	var b = im._21 * vAd[0] + im._22 * vBd[0];

	var c = im._11 * vAd[1] + im._12 * vBd[1];
	var d = im._21 * vAd[1] + im._22 * vBd[1];

	var wu = uvs[0].u * w;
	var hv = uvs[0].v * h;
	var du = wu * a + hv * b;
	var dv = wu * c + hv * d;

	g.save();

	g.beginPath();
	g.moveTo(poss[0].x, poss[0].y);
	g.lineTo(poss[1].x, poss[1].y);
	g.lineTo(poss[2].x, poss[2].y);
	g.clip();

	g.transform(a, c, b, d, poss[0].x - du, poss[0].y - dv);

	// bounds
	var bx = [wu, wu+vA[0], wu+vB[0]];
	var by = [hv, hv+vA[1], hv+vB[1]];

	bx.sort(P3D.num_cmp);
	by.sort(P3D.num_cmp);

	var bw = bx[2] - bx[0];
	var bh = by[2] - by[0];

	if ((bx[0]+bw) <= (w-1)) bw++;
	if ((by[0]+bh) <= (h-1)) bh++;
	if (bx[0] >= 1) {bx[0]--; bw++;}
	if (by[0] >= 1) {by[0]--; bh++;}

	g.drawImage(this.texture, bx[0], by[0], bw, bh, bx[0], by[0], bw, bh);

	if (shade_clr) {
		g.fillStyle = shade_clr;
		g.fillRect(bx[0], by[0], bw, bh);
	}

	g.restore();

	return true;
}

P3D.drawTestByIndexBuffer = function(pos_buf, ix_buf, culling) {
	var g = this.g;

	if ((ix_buf.length%3) != 0)
		throw "invalid index buffer length!";

	var len = ix_buf.length/3;

	var i, ibase, vbase;
	var poss = [{},{},{}];
	g.strokeWidth = 1;
	for (i = 0, ibase = 0;i < len;++i)
	{
		vbase = ix_buf[ibase++] << 2;
		poss[0].x = pos_buf[vbase++];
		poss[0].y = pos_buf[vbase  ];

		vbase = ix_buf[ibase++] << 2;
		poss[1].x = pos_buf[vbase++];
		poss[1].y = pos_buf[vbase  ];

		vbase = ix_buf[ibase++] << 2;
		poss[2].x = pos_buf[vbase++];
		poss[2].y = pos_buf[vbase  ];

		// z component of cross product < 0 ?

		var Ax = poss[1].x - poss[0].x;
		var Ay = poss[1].y - poss[0].y;
		var Cx = poss[2].x - poss[1].x;
		var Cy = poss[2].y - poss[1].y;

		var cull = ( (((Ax * Cy) - (Ay * Cx))*culling) < 0);

		g.beginPath();
		g.strokeStyle = cull ? "#592" : "#0f0";
		g.moveTo(poss[0].x, poss[0].y);
		g.lineTo(poss[1].x, poss[1].y);
		g.lineTo(poss[2].x, poss[2].y);
		g.lineTo(poss[0].x, poss[0].y);
		g.stroke();
	}
}

P3D.drawByIndexBuffer = function(pos_buf, ix_buf, tx_buf, culling, z_clip) {
	var w, h;
	var color_polygon = !this.texture;
	if (this.texture) {
		w = this.texture.width;
		h = this.texture.height;
	}

	var g = this.g;
	var m = new M22();

	if (!culling) culling = 0;

	if ((ix_buf.length%3) != 0)
		throw "invalid index buffer length!";

	var i, ibase, vbase, tbase, poss = [{},{},{}];
	var len = ix_buf.length/3;
	var uv_0u, uv_0v, uv_1u, uv_1v, uv_2u, uv_2v;

	for (i = 0, ibase = 0;i < len;++i)
	{
		tbase = ix_buf[ibase++] << 1
		vbase = tbase << 1;
		poss[0].x = pos_buf[vbase++]; uv_0u = tx_buf[tbase++];
		poss[0].y = pos_buf[vbase++]; uv_0v = tx_buf[tbase];
		if (z_clip && (pos_buf[vbase] < 0 || pos_buf[vbase] > 1)) {ibase += 2; continue;}

		tbase = ix_buf[ibase++] << 1
		vbase = tbase << 1;
		poss[1].x = pos_buf[vbase++]; uv_1u = tx_buf[tbase++];
		poss[1].y = pos_buf[vbase++]; uv_1v = tx_buf[tbase];
		if (z_clip && (pos_buf[vbase] < 0 || pos_buf[vbase] > 1)) {++ibase; continue;}

		tbase = ix_buf[ibase++] << 1
		vbase = tbase << 1;
		poss[2].x = pos_buf[vbase++]; uv_2u = tx_buf[tbase++];
		poss[2].y = pos_buf[vbase++]; uv_2v = tx_buf[tbase];
		if (z_clip && (pos_buf[vbase] < 0 || pos_buf[vbase] > 1)) {continue;}

		var vAd = [ poss[1].x - poss[0].x , poss[1].y - poss[0].y ];
		var vBd = [ poss[2].x - poss[0].x , poss[2].y - poss[0].y ];

		var vCd = [ poss[2].x - poss[1].x , poss[2].y - poss[1].y ];
		
		// z component of cross product < 0 ?
		if( (((vAd[0] * vCd[1]) - (vAd[1] * vCd[0]))*culling) < 0)
			continue;

		if (color_polygon) {
			g.fillStyle = uv_0u;

			g.beginPath();
			g.moveTo(poss[0].x, poss[0].y);
			g.lineTo(poss[1].x, poss[1].y);
			g.lineTo(poss[2].x, poss[2].y);
			g.fill();
			continue;
		}

		var vA = [ uv_1u - uv_0u , uv_1v - uv_0v ];
		var vB = [ uv_2u - uv_0u , uv_2v - uv_0v ];

		vA[0] *= w;
		vA[1] *= h;

		vB[0] *= w;
		vB[1] *= h;

		m._11 = vA[0];
		m._12 = vA[1];
		m._21 = vB[0];
		m._22 = vB[1];

		var im = m.getInvert();
		if (!im) { continue;}

		var a = im._11 * vAd[0] + im._12 * vBd[0];
		var b = im._21 * vAd[0] + im._22 * vBd[0];

		var c = im._11 * vAd[1] + im._12 * vBd[1];
		var d = im._21 * vAd[1] + im._22 * vBd[1];

		var wu = uv_0u * w;
		var hv = uv_0v * h;
		var du = wu * a + hv * b;
		var dv = wu * c + hv * d;

		g.save();

		g.beginPath();
		g.moveTo(poss[0].x, poss[0].y);
		g.lineTo(poss[1].x, poss[1].y);
		g.lineTo(poss[2].x, poss[2].y);
		g.clip();
		g.transform(a, c, b, d, poss[0].x - du, poss[0].y - dv);

		// bounds
		var bx = [wu, wu+vA[0], wu+vB[0]];
		var by = [hv, hv+vA[1], hv+vB[1]];

		bx.sort(P3D.num_cmp);
		by.sort(P3D.num_cmp);

		var bw = bx[2] - bx[0];
		var bh = by[2] - by[0];

		if ((bx[0]+bw) <= (w-1)) bw++;
		if ((by[0]+bh) <= (h-1)) bh++;
		if (bx[0] >= 1) {bx[0]--; bw++;}
		if (by[0] >= 1) {by[0]--; bh++;}

		g.drawImage(this.texture, bx[0], by[0], bw, bh, bx[0], by[0], bw, bh);
/*
		if (shade_clr) {
			g.fillStyle = shade_clr;
			g.fillRect(bx[0], by[0], bw, bh);
		}
*/
		g.restore();

	}

}

function Vec3(_x, _y, _z)
{
	this.x = _x || 0;
	this.y = _y || 0;
	this.z = _z || 0;
}

Vec3.prototype = {
	zero: function() {
		this.x = this.y = this.z = 0;
	},

	sub: function(v) {
		this.x -= v.x;
		this.y -= v.y;
		this.z -= v.z;

		return this;
	},

	add: function(v) {
		this.x += v.x;
		this.y += v.y;
		this.z += v.z;

		return this;
	},

	copyFrom: function(v) {
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;

		return this;
	},

	norm:function() {
		return Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z);
	},

	normalize: function() {
		var nrm = Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z);
		if (nrm != 0)
		{
			this.x /= nrm;
			this.y /= nrm;
			this.z /= nrm;
		}
		return this;
	},

	smul: function(k) {
		this.x *= k;
		this.y *= k;
		this.z *= k;

		return this;
	},

	dpWith: function(v)	{
		return this.x*v.x + this.y*v.y + this.z*v.z;
	},

	cp: function(v, w) {
		this.x = (w.y * v.z) - (w.z * v.y);
		this.y = (w.z * v.x) - (w.x * v.z);
		this.z = (w.x * v.y) - (w.y * v.x);

		return this;
	},

	toString: function() {
		return this.x + ", " + this.y + "," + this.z;
	}
}

function M44(cpy)
{
	if (cpy)
		this.copyFrom(cpy);
	else {
		this.ident();
	}
}

M44.prototype = {
	ident: function() {
			  this._12 = this._13 = this._14 = 0;
		this._21 =       this._23 = this._24 = 0;
		this._31 = this._32 =       this._34 = 0;
		this._41 = this._42 = this._43 =       0;

		this._11 = this._22 = this._33 = this._44 = 1;

		return this;
	},

	copyFrom: function(m) {
		this._11 = m._11;
		this._12 = m._12;
		this._13 = m._13;
		this._14 = m._14;

		this._21 = m._21;
		this._22 = m._22;
		this._23 = m._23;
		this._24 = m._24;

		this._31 = m._31;
		this._32 = m._32;
		this._33 = m._33;
		this._34 = m._34;

		this._41 = m._41;
		this._42 = m._42;
		this._43 = m._43;
		this._44 = m._44;

		return this;
	},

	transVec3: function(out, x, y, z) {
		out[0] = x * this._11 + y * this._21 + z * this._31 + this._41;
		out[1] = x * this._12 + y * this._22 + z * this._32 + this._42;
		out[2] = x * this._13 + y * this._23 + z * this._33 + this._43;
		out[3] = x * this._14 + y * this._24 + z * this._34 + this._44;
	},

	transVec3Rot: function(out, x, y, z) {
		out[0] = x * this._11 + y * this._21 + z * this._31;
		out[1] = x * this._12 + y * this._22 + z * this._32;
		out[2] = x * this._13 + y * this._23 + z * this._33;
	},

	perspectiveLH: function(vw, vh, z_near, z_far) {
		this._11 = 2.0*z_near/vw;
		this._12 = 0;
		this._13 = 0;
		this._14 = 0;

		this._21 = 0;
		this._22 = 2*z_near/vh;
		this._23 = 0;
		this._24 = 0;

		this._31 = 0;
		this._32 = 0;
		this._33 = z_far/(z_far-z_near);
		this._34 = 1;

		this._41 = 0;
		this._42 = 0;
		this._43 = z_near*z_far/(z_near-z_far);
		this._44 = 0;

		return this;
	},

	lookAtLH: function(aUp, aFrom, aAt) {
		var aX = new Vec3();
		var aY = new Vec3();
	
		var aZ = new Vec3(aAt.x, aAt.y, aAt.z);
		aZ.sub(aFrom).normalize();
	
		aX.cp(aUp, aZ).normalize();
		aY.cp(aZ, aX);
	
		this._11 = aX.x;  this._12 = aY.x;  this._13 = aZ.x;  this._14 = 0;
		this._21 = aX.y;  this._22 = aY.y;  this._23 = aZ.y;  this._24 = 0;
		this._31 = aX.z;  this._32 = aY.z;  this._33 = aZ.z;  this._34 = 0;
	
		this._41 = -aFrom.dpWith(aX);
		this._42 = -aFrom.dpWith(aY);
		this._43 = -aFrom.dpWith(aZ);
		this._44 = 1;
	
	    return this;
	},

	mul: function(A, B) {
		this._11 = A._11*B._11  +  A._12*B._21  +  A._13*B._31  +  A._14*B._41;
		this._12 = A._11*B._12  +  A._12*B._22  +  A._13*B._32  +  A._14*B._42;
		this._13 = A._11*B._13  +  A._12*B._23  +  A._13*B._33  +  A._14*B._43;
		this._14 = A._11*B._14  +  A._12*B._24  +  A._13*B._34  +  A._14*B._44;

		this._21 = A._21*B._11  +  A._22*B._21  +  A._23*B._31  +  A._24*B._41;
		this._22 = A._21*B._12  +  A._22*B._22  +  A._23*B._32  +  A._24*B._42;
		this._23 = A._21*B._13  +  A._22*B._23  +  A._23*B._33  +  A._24*B._43;
		this._24 = A._21*B._14  +  A._22*B._24  +  A._23*B._34  +  A._24*B._44;

		this._31 = A._31*B._11  +  A._32*B._21  +  A._33*B._31  +  A._34*B._41;
		this._32 = A._31*B._12  +  A._32*B._22  +  A._33*B._32  +  A._34*B._42;
		this._33 = A._31*B._13  +  A._32*B._23  +  A._33*B._33  +  A._34*B._43;
		this._34 = A._31*B._14  +  A._32*B._24  +  A._33*B._34  +  A._34*B._44;

		this._41 = A._41*B._11  +  A._42*B._21  +  A._43*B._31  +  A._44*B._41;
		this._42 = A._41*B._12  +  A._42*B._22  +  A._43*B._32  +  A._44*B._42;
		this._43 = A._41*B._13  +  A._42*B._23  +  A._43*B._33  +  A._44*B._43;
		this._44 = A._41*B._14  +  A._42*B._24  +  A._43*B._34  +  A._44*B._44;

		return this;
	},

	translate: function(x, y, z) {
		this._11 = 1;  this._12 = 0;  this._13 = 0;  this._14 = 0;
		this._21 = 0;  this._22 = 1;  this._23 = 0;  this._24 = 0;
		this._31 = 0;  this._32 = 0;  this._33 = 1;  this._34 = 0;

		this._41 = x;  this._42 = y;  this._43 = z;  this._44 = 1;
		return this;
	},

	transpose33: function() {
		var t;

		t = this._12;
		this._12 = this._21;
		this._21 = t;

		t = this._13;
		this._13 = this._31;
		this._31 = t;

		t = this._23;
		this._23 = this._32;
		this._32 = t;

		return this;
	},

	// OpenGL style rotation
	glRotate: function(angle, x, y, z) {
		var s = Math.sin( angle );
		var c = Math.cos( angle );

		var xx = x * x;
		var yy = y * y;
		var zz = z * z;
		var xy = x * y;
		var yz = y * z;
		var zx = z * x;
		var xs = x * s;
		var ys = y * s;
		var zs = z * s;
		var one_c = 1.0 - c;
/*
		this._11 = (one_c * xx) + c;
		this._21 = (one_c * xy) - zs;
		this._31 = (one_c * zx) + ys;
		this._41 = 0;

		this._12 = (one_c * xy) + zs;
		this._22 = (one_c * yy) + c;
		this._32 = (one_c * yz) - xs;
		this._42 = 0;

		this._13 = (one_c * zx) - ys;
		this._23 = (one_c * yz) + xs;
		this._33 = (one_c * zz) + c;
		this._43 = 0;

		this._14 = 0;
		this._24 = 0;
		this._34 = 0;
		this._44 = 1;
*/

		this._11 = (one_c * xx) + c;
		this._12 = (one_c * xy) - zs;
		this._13 = (one_c * zx) + ys;
		this._14 = 0;

		this._21 = (one_c * xy) + zs;
		this._22 = (one_c * yy) + c;
		this._23 = (one_c * yz) - xs;
		this._24 = 0;

		this._31 = (one_c * zx) - ys;
		this._32 = (one_c * yz) + xs;
		this._33 = (one_c * zz) + c;
		this._34 = 0;

		this._41 = 0;
		this._42 = 0;
		this._43 = 0;
		this._44 = 1;

		return this;
	}

}

// matrix 2x2
function M22()
{
	this._11 = 1;
	this._12 = 0;
	this._21 = 0;
	this._22 = 1;
}

M22.prototype.getInvert = function()
{
	var out = new M22();
	var det = this._11 * this._22 - this._12 * this._21;
	if (det > -0.0001 && det < 0.0001)
		return null;

	out._11 = this._22 / det;
	out._22 = this._11 / det;

	out._12 = -this._12 / det;
	out._21 = -this._21 / det;

	return out;
}	
