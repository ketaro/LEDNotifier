import { Component, OnInit, Input } from '@angular/core';
import { Location } from '@angular/common';

import { LEDService } from '../led.service';
import { Palette } from '../palette';

@Component({
  selector: 'app-palette-detail',
  templateUrl: './palette-detail.component.html',
  styleUrls: ['./palette-detail.component.css']
})
export class PaletteDetailComponent implements OnInit {
    
    @Input() palette: Palette;

    constructor(
        private ledService: LEDService
    ) { }

    ngOnInit() {
    }
    
    save(): void {
        this.ledService.updatePalette( this.palette )
            .subscribe();
    }

}
