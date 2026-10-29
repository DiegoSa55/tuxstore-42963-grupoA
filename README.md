# Tux Store · servidor `<nombre-del-servidor>`

![Tux Store CI](../../actions/workflows/tuxstore.yml/badge.svg)

Gestor de paquetes de consola hecho en C++ con una **lista enlazada** (catalogo),
una **cola de prioridad** (descargas) y una **pila** (disco), con persistencia en `tuxstore.txt`.

Estructura de Datos · Evaluacion por competencias C2 · 2026-20 · NRC `<NRC>` · Grupo `<letra>`

## Equipo

| Integrante | Usuario de GitHub | Rol | Requisitos |
|---|---|---|---|
|  |  | Lider (scrum) |  |
|  |  | Dev · catalogo | R1, R5 |
|  |  | Dev · descargas | R2, R3 |
|  |  | Dev · disco y archivo | R4, R6 |
|  |  | QA y documentacion | R7, pruebas |

Tablero del equipo: `<enlace a GitHub Projects>` · Presentacion: `<enlace>`

## Como compilar y ejecutar

1. Abrir `tuxstore.cpp` en Dev-C++ y compilar (F11). Si aparece un aviso, activar C++11 en
   Herramientas > Opciones del compilador.
2. Ejecutar y escribir `ayuda`.
3. Para guardar la salida de una secuencia: `tuxstore.exe < pruebas\secuencia.txt > pruebas\salida.txt`

## Pruebas automaticas (GitHub Actions)

En cada push, GitHub compila el programa, ejecuta `pruebas/secuencia.txt` y revisa que cada linea
de `pruebas/esperado.txt` aparezca en la salida. El kit trae el ejemplo del servidor de prueba.
Cuando carguen su ficha, reemplacen:

- `pruebas/secuencia.txt` por la secuencia oficial de su ficha;
- `pruebas/esperado.txt` por las lineas de `fila` y `procesar` de su traza a mano.

Si la marca sale roja, abran la pestana **Actions** y vean que linea falta.

## Versiones

| Etiqueta | Entrega | Contenido |
|---|---|---|
| v0.1 | E1 | Diseno, traza a mano, kit con la ficha |
| v0.5 | E2 | R1 a R4 |
| v1.0 | E3 | R1 a R7, informe, pruebas, presentacion |
