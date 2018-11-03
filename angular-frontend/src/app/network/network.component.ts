import { Component, OnInit } from '@angular/core';
import { Location } from '@angular/common';

import { SettingsService } from '../settings.service';
import { Network } from '../network';

@Component({
	selector: 'app-network',
	templateUrl: './network.component.html',
	styleUrls: ['./network.component.css']
})
export class NetworkComponent implements OnInit {

	network: Network;

	constructor(
		private settingsService: SettingsService,
		private location: Location
	) { }

	ngOnInit() {
		this.getNetwork();
	}

	getNetwork():void {
		this.settingsService.getNetwork()
			.subscribe( network => this.network = network );
	}

	save():void {
		this.settingsService.updateNetwork(this.network)
		.subscribe( () => this.goBack() );
	}

	goBack():void {
		 this.location.back();
	}

}
