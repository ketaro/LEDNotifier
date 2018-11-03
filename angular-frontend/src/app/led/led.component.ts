import { Component, OnInit } from '@angular/core';
import { Location } from '@angular/common';

import { LEDService } from '../led.service';
import { LED } from '../led';

@Component({
	selector: 'app-led',
	templateUrl: './led.component.html',
	styleUrls: ['./led.component.css']
})
export class LedComponent implements OnInit {

	led: LED;

	constructor(
		private ledService: LEDService,
		private location: Location
	) { }


	ngOnInit() {
		this.getLEDs();
	}


	getLEDs():void {
		this.ledService.getLEDs()
			.subscribe( led => this.led = led );
	}


	update():void {
		console.log('click!');
		this.ledService.updateDisplay( this.led )
			.subscribe( );
	}

	goBack():void {
		this.location.back();
	}

}
