# Тестовое задание в WarGaming

## Задание 1

Решение представлено [здесь](someSolution.com).  
Концепция решения: применяем побитовое И для проверяемого на четность аргумента и единицы. В таком случае
получим 1 тогда и только тогда, когда нулевой бит проверяемого числа будет 
равен 1, т. е. в разложении на сумму степеней двойки будет присутствовать слагаемое
со значением 1 -> число будет нечетным.  
Применяем логическое отрицание к результату побитового И, чтобы получить значение, соответствующей
ожидаемой семантике функции. 

Недостатки данного решения по сравнению с приведенным в задании:

* Более высокий уровень сложности визуального и смыслового восприятия.

Достоинства данного решения по сравнению с приведенным в задании:

* Так и хочется сказать "Ну, мы же работаем на уровне БАЙТЕКОВ, очевидно,
  данный вариант будет быстрее".  
  Однако заявлять об этом, руководствуясь только интуицией, - сомнительная затея,
  поэтому посмотрим на то, что получается в различных реализациях в Ассемблерном
  коде для двух данных функций. 

### x86-64 gcc 11.2
Предлагаемая функция:

    ```cpp
    bool isEven(int value)
    {
        return value % 2 == 0;
    }
    ```

Ассемблерный код:
    
    ```asm
    isEven(int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], edi
        mov     eax, DWORD PTR [rbp-4]
        and     eax, 1
        test    eax, eax
        sete    al
        pop     rbp
        ret
    ```
  
Собственная функция: 

    ```cpp
    bool isEven(int value) 
    {
        return !(value & 1);
    }
    ```

Ассемблерный код:

    ```asm
    isEven(int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], edi
        mov     eax, DWORD PTR [rbp-4]
        and     eax, 1
        test    eax, eax
        setne   al
        pop     rbp
        ret
    ```
Как можно заметить, для последней версии gcc то, что получается в Ассемблерном коде,
*буквально не отличается* в обоих вариантах.

### x64 msvc v19.30
Предлагаемая функция:

    ```asm
    tv66 = 0
    value$ = 32
    bool isEven(int) PROC                             ; isEven
    $LN5:
    mov     DWORD PTR [rsp+8], ecx
    sub     rsp, 24
    mov     eax, DWORD PTR value$[rsp]
    cdq
    and     eax, 1
    xor     eax, edx
    sub     eax, edx
    test    eax, eax
    jne     SHORT $LN3@isEven
    mov     DWORD PTR tv66[rsp], 1
    jmp     SHORT $LN4@isEven
    $LN3@isEven:
    mov     DWORD PTR tv66[rsp], 0
    $LN4@isEven:
    movzx   eax, BYTE PTR tv66[rsp]
    add     rsp, 24
    ret     0
    bool isEven(int) ENDP                             ; isEven
    ```

Собственная функция:  

    ```asm
    tv66 = 0
    value$ = 32
    bool isEven(int) PROC                             ; isEven
    $LN5:
    mov     DWORD PTR [rsp+8], ecx
    sub     rsp, 24
    mov     eax, DWORD PTR value$[rsp]
    and     eax, 1
    test    eax, eax
    jne     SHORT $LN3@isEven
    mov     DWORD PTR tv66[rsp], 1
    jmp     SHORT $LN4@isEven
    $LN3@isEven:
    mov     DWORD PTR tv66[rsp], 0
    $LN4@isEven:
    movzx   eax, BYTE PTR tv66[rsp]
    add     rsp, 24
    ret     0
    bool isEven(int) ENDP                             ; isEven
    ```

В случае с MSVC для предлагаемого в задании варианта выполняются следующие
команды: cdq, and, xor и sub, тогда как в приведенной мной реализации вместо
всего этого выполняется только одна команда - and. 

### Так что в итоге?
А в итоге можно сказать, что в каких-то реализациях функция, использующая
побитовое И будет выигрывать по производительности, а в каких-то будет стоять
наравне с функцией, использующей взятие остатка от деления на 2.

**ОДНАКО**. Если для какого угодно компилятора включить *по крайней мере 
минимальные* оптимизации и, например, для MSVC использовать O1, то Ассемблерный код будет выглядеть
одинаково для обоих вариантов.

И я **ИСКРЕННЕ** не верю, что Release-версия проекта будет собираться без оптимизаций, 
достаточных для того, чтобы обе функции стали одинаково эффективны.

Так что при рассмотрении сферического коня в вакууме можно сказать, что в каких-то
реализациях моя функция будет более быстродействующей, но в реальности это
практически никогда не будет правдой.

##Задание 3
Решение представлено [здесь](someSolution.com). 

Решение задания представлено функцией wargamingSort.

В том случае, если в сортируемом массиве будет меньше 16 элементов, будет использована
сортировка вставками. В противном случае - быстрая сортировка.

Такой выбор сделан в силу того, что постоянные множители, которые скрываются за асимптотическими
обозначениями сложности быстрой сортировки, больше, чем множители, которые
скрываются за асимптотическими обозначениями для сортировки вставками. Поэтому для 
массивов малых размеров сортировка вставками будет работать быстрее, чем быстрая сортировка, 
несмотря на то, что сортировка вставками работает за O(n^2), а быстрая сортировка в среднем
случае работает за O(n * log(n)).

При этом сортировка вставками реализована таким образом, что опорный элемент выбирается как
элемент, индекс которого равен медиане трех случайных индексов. Это позволяет минимизировать
шанс того, что быстрая сортировка будет иметь сложность O(n ^ 2), что происходило бы
в случае с отсортированным массивом, если бы в качестве опорного элемента стабильно выбирался
бы самый правый элемент.

Таким образом, у приведенной функции сортировки есть следующие преимущества:

* Массивы маленьких размеров сортируются достаточно быстро благодаря использованию
  сортировки вставками, у сложности которой маленькие константные множители.  

* Для массивов большего размера мы получаем сложность O(n * log(n)) благодаря быстрой
  сортировке

* Даже если массив отсортирован, что является худшим случаем для быстрой сортировки,
  мы все еще избегаем сложности O(n ^ 2) благодаря опорному элементу с индексом-медианой
  трех случайных индексов.