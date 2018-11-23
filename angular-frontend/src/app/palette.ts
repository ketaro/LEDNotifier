export class Palette {
    name: string;
    colors: Array<string>;
}

// Convert RGB color values into an integer
export function rgbToInt( r, g, b ): number {
	return (r << 16) + (g << 8) + (b);
}


// Convert an integer into RGB color values
export function intToRGB( n:number ): Array<number> {
	let r = ( n & 0xff0000 ) >> 16;
	let g = ( n & 0x00ff00 ) >> 8;
	let b = ( n & 0x0000ff );
	
	return [ r, g, b ];
}

