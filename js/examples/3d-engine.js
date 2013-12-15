( function () {

	var res = 25, res3 = res * res * res,
	i = 0, x = 0, y = 0, z = 0, s, size, sizeHalf,
	vx, vy, vz, rsx, rcx, rsy, rcy, rsz, rcz,
	xy, xz, yx, yz, zx, zy,
	cx = 0, cy = 0, cz = 1, rx = 1, ry = 1, rz = 0,
	t, t1, t2, t3,
	sin = Math.sin, cos = Math.cos, pi = Math.PI * 3,
	mouseX = 0, mouseY = 0, color,
	doc = document, body = doc.body,
	canvas, context, mesh = [],
	width = innerWidth,
	height = innerHeight,
	widthHalf = width / 2,
	heightHalf = height / 2;

	body.style.margin = '0px';
	body.style.overflow = 'hidden';

	canvas = doc.body.children[0];
	canvas.width = width;
	canvas.height = height;

	context = canvas.getContext( '2d' );
	context.translate( widthHalf, heightHalf );

	doc.onmousemove = function ( event ) {

		mouseX = ( event.clientX - widthHalf ) / 1000;
		mouseY = ( event.clientY + heightHalf ) / 1000;

	};

	while ( i++ < res3 ) {

		mesh.push( x / res - 0.5 );
		mesh.push( y / res - 0.5 );
		mesh.push( z / res - 0.5 );

		z = i % res;
		y = !z ? ++y %res : y;
		x = !z && !y ? ++x : x;

	}

	setInterval( function () {

		context.clearRect( - widthHalf, - heightHalf, width, height );

		cx += ( mouseX - cx ) / 10;
		cz += ( mouseY - cz ) / 10;

		t = new Date().getTime();
		t1 = sin( t / 20000 ) * pi;
		t2 = sin( t / 10000 ) * pi;
		t3 = sin( t / 15000 ) * pi;

		rx = t / 10000;

		rsx = sin( rx ); rcx = cos( rx );
		rsy = sin( ry ); rcy = cos( ry );
		rsz = sin( rz ); rcz = cos( rz );

		i = 0;

		while ( ( i += 3 ) < res3 * 3 ) {

			x = mesh[ i ];
			y = mesh[ i + 1 ];
			z = mesh[ i + 2 ];
			s = sin( t1 + x * t1 ) + sin( t2 + y * t2 ) + sin( t3 + z * t3 );

			if ( s >= 0 ) {

				xy = rcx * y - rsx * z;
				xz = rsx * y + rcx * z;

				yz = rcy * xz - rsy * x;
				yx = rsy * xz + rcy * x;

				zx = rcz * yx - rsz * xy;
				zy = rsz * yx + rcz * xy;

				vx = zx - cx;
				vy = zy - cy;
				vz = yz + cz;

				if ( vz > 0 ) {

					color = ( 64 / vz ) >> 0;
					context.fillStyle = 'rgb('+ ( color - 16 ) + ','+ ( color * 2 - 128 ) + ','+ ( color + 64 ) + ')';

					size = s * 30 / vz;
					sizeHalf = size / 2;

					context.fillRect( ( vx / vz ) * widthHalf - sizeHalf, ( vy / vz ) * widthHalf - sizeHalf, size, size );

				}

			}
		}

	}, 16 );

} )();