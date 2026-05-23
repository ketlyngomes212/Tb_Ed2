    // FASE 3: voa reto para esquerda
    gotoxy(1, 22); printf("                                                                ");
    gotoxy(1, 22); printf("Voando de %s para %s...", av.origem, av.destino);
    while (x > 0) {
        gotoxy(px, py); printf("      ");
        gotoxy(x,   3); printf("{=|}<<");
        espera(80);
        px = x; py = 3; x -= 2;
    }

    // limpa antes de resetar
    gotoxy(px, 3); printf("      ");  // ← apaga último avião da fase 3

    // FASE 4: desce para esquerda
    x = 58; y = 3; px = x; py = y;
    while (y < 19) {