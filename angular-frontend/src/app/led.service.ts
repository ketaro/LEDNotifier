import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { MessageService } from './message.service';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { LED } from './led';
import { Palette, intToRGB, rgbToInt } from './palette';

export const PALETTES: Palette[] = [
    { name: "Test", 
      colors: [ "#00f00d", "#ff0000", "#00ff00", "#0000ff", 
                "#cccccc", "#ffffff", "#facebc", "#f00bad",
                "#00f00d", "#ff0000", "#00ff00", "#0000ff", 
                "#cccccc", "#ffffff", "#facebc", "#f00bad",
              ]
    }
]

@Injectable({
	providedIn: 'root'
})
export class LEDService {
	private serviceUrl = 'http://10.0.80.188:8080/api/led';

	constructor(
		private http: HttpClient,
		private messageService: MessageService
	) { }


	getLEDs(): Observable<LED> {
		this.log( '[LEDService] getLEDs()' );
		return this.http.get<LED>( this.serviceUrl + '/display' )
				.pipe(
					tap( led => this.log( '[LEDService] fetched LED display' ) ),
					catchError( this.handleError( 'getLEDs', { leds: 0, delay: 0, brightness: 0 } ) )
				);
	}
	
	
	display( mode: number, duration: number ):void {
		let data = new FormData();
		data.append( 'mode', mode.toString() );
		data.append( 'duration', duration.toString() );

		this.log( "Setting display" );
		this.http.post( this.serviceUrl + '/display', data )
			.subscribe(
				response => console.log( response ),
				err      => console.log( err )
			);
//			.pipe(
//				tap( _ => this.log( 'set display' ) ),
//				catchError( this.handleError<any>('display') )
//			);
	}
	
	updateDisplay( led:LED ):Observable<any> {
		let data = new FormData();

		data.append( 'leds', led.leds.toString() );
		data.append( 'delay', led.delay.toString() );
		data.append( 'brightness', led.brightness.toString() );

		return this.http.post( this.serviceUrl, data )
			.pipe(
				tap( _ => this.log( '[LEDService] update led display' ) ),
				catchError( this.handleError<any>('updateDisplay') )
			);
	}


    // Retrieve the custom palettes from the API
	getPalettes(): Observable<Palette[]> {
    	this.log( '[LEDService] getPalettes()' );
    	
    	return this.http.get<Palette[]>( this.serviceUrl + '/palettes' )
    	        .pipe(
        	        tap( _ => this.log( '[LEDService] fetched palettes' )),
        	        catchError( this.handleError( 'getPalettes', [] ))
    	        );
    	
    	return of( PALETTES );
	}
	
	
    // Update a custom palette via the API
    updatePalette( palette:Palette ):Observable<any> {
        var self = this;
        let data = new FormData();
        
        data.append( 'name', palette.name );
        
        // Convert each #rrggbb color code to an integer
        palette.colors.forEach(function( item, index, array) {
            let m = item.match(/\#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})/);
            let r = parseInt( m[1], 16 );
            let g = parseInt( m[2], 16 );
            let b = parseInt( m[3], 16 );
            
            let cint = rgbToInt( r, g, b );

            console.log("color " + item + " -- " + cint );
            data.append( 'palette', cint.toString() );
        });
        
        return this.http.post( this.serviceUrl + '/palette', data )
            .pipe(
                tap( _ => this.log( '[LEDService] update palette' ) ),
                catchError( this.handleError<any>('updatePalette') )
            );
    }


	showAlert(): void {
		this.display( 6, 10 );
	}

	private log( message: string ) {
		this.messageService.add( `[LED] ${message}` );
	}

	/**
	* Handle Http operation that failed.
	* Let the app continue.
	* @param operation - name of the operation that failed
	* @param result - optional value to return as the observable result
	*/
	private handleError<T> (operation = 'operation', result?: T) {
		return (error: any): Observable<T> => {
			// TODO: send the error to remote logging infrastructure
			console.error(error); // log to console instead

			// TODO: better job of transforming error for user consumption
			this.log(`${operation} failed: ${error.message}`);

			// Let the app keep running by returning an empty result.
			return of(result as T);
		};
	}

}
