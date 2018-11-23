import { Component, OnInit } from '@angular/core';

import { LEDService } from '../led.service';
import { Palette } from '../palette';

@Component({
  selector: 'app-palette',
  templateUrl: './palette.component.html',
  styleUrls: ['./palette.component.css']
})
export class PaletteComponent implements OnInit {
    
    palettes: Palette[];
    selectedPalette: Palette;

    constructor(
        private ledService: LEDService
    ) { }
    
    ngOnInit() {
        this.getPalettes();
    }
    
    onSelect( palette: Palette ): void {
        this.selectedPalette = palette;
    }
    
    getPalettes(): void {
        this.ledService.getPalettes()
            .subscribe( palettes => this.palettes = palettes );
    }

}
