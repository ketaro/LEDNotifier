import { Component, OnInit } from '@angular/core';
import { SettingsService } from '../settings.service';
import { Network } from '../network';
import { NETWORK } from '../mock-network';

@Component({
  selector: 'app-network',
  templateUrl: './network.component.html',
  styleUrls: ['./network.component.css']
})
export class NetworkComponent implements OnInit {

  network: Network;

  constructor(private settingsService: SettingsService) { }

  ngOnInit() {
    this.getNetwork();
  }

  getNetwork():void {
    this.settingsService.getNetwork()
      .subscribe( network => this.network = network );
  }

}
