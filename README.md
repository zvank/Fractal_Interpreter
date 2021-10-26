# Fractal Interpreter
*Scroll down for the English version*
## Как пользоваться
На вход программе подаётся описание фрактала (содержится ниже)

Важные моменты:
  - Рендерится только область \[0, 0.5\] x \[0, 0.5\].
  - При использовании нескольких аттракторов стоит создавать их примерно одинакового размера, иначе возможны проблемы с рендерингом.
## Описание фрактала
```
!add = n1, n2;  - опциональная строчка с дополнительными аттракторами, имя основного - 'main'

# описание аттракторов
n1 = {
    n1 [ 1 1 0 1 ] [] [ 0.5 ] + 
    ^   ^           ^   ^
    |   |           |   |
    |   |           |  коэф. домножения - можно [] для 1
    |   |           |
    |   |       вектор параллельного переноса - можно [] для нулевого
    |   |
    |   матрица - можно [] для E
    |
    аттрактор - обязателен

    n2 [] [] []
}

n2 = { n2 [] [] [ 0.5 ] + n2 [] [ 0.5 0 ] [ 0.5 ] + n2 [] [ 0.25 0.25 ] [ 0.5 ] }

main = { n2 [] [] [ 0.5 ] + n1 [] [ 0 1 ] [ 0.5 ] + main [] [ 0.5 0 ] [ 0.5 ] }

exec - 'выполнить', рисуется фрактал, программа завершает работу.

# ОБЯЗАТЕЛЬНО числа отделять пробелами от квадратных скобок.
```
## How to use
This application may help you in drawing different fractals using their decription (scroll down)

Important points:
  - Only [0, 0.5] x [0, 0.5] area is rendered.
  - All the attractors should be approximately the same size, otherwise rendering ploblems may occure
## Fractal decription
```
!add = n1, n2;  - optional, if you need additional attractors, name of the main one is 'main'

# attractors decription

n1 = {
    n1 [ 1 1 0 1 ] [] [ 0.5 ] + 
    ^   ^           ^   ^
    |   |           |   |
    |   |           |  multiply coefficient - default [] is for 1
    |   |           |
    |   |       translation vector - default [] is for ( 0, 0 )
    |   |
    |   matrix - default [] if for I
    |
    attractor name is mandatory

    n2 [] [] []
}

n2 = { n2 [] [] [ 0.5 ] + n2 [] [ 0.5 0 ] [ 0.5 ] + n2 [] [ 0.25 0.25 ] [ 0.5 ] }

main = { n2 [] [] [ 0.5 ] + n1 [] [ 0 1 ] [ 0.5 ] + main [] [ 0.5 0 ] [ 0.5 ] }

exec - 'execute', main attractor is pictured, the programme stops.

# you should print spaces between numbers and square brackets.
```
