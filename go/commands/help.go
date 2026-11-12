package commands

import "fmt"

func PrintHelp() {
	fmt.Print(`Запуск:
./build/lab3go --file файл.json --query 'команда данные'

Один файл хранит одну структуру. Сначала выполните команду инициализации.
Повторная инициализация очищает структуру в выбранном файле.
Индексы начинаются с 0. Значения с пробелами заключайте в двойные кавычки.

FInitList | FAddFirst значение | FAddEnd значение
FAddIndex значение индекс | FAddBefore индекс значение | FAddAfter индекс значение
FDeleteFirst | FDeleteEnd | FDeleteIndex индекс | FDeleteValue значение
FDeleteBefore индекс | FDeleteAfter индекс
FPrint | FPrintBack | FPrintAddresses | FFind значение

LInitList | LAddFirst значение | LAddEnd значение
LAddIndex значение индекс | LAddBefore индекс значение | LAddAfter индекс значение
LDeleteFirst | LDeleteEnd | LDeleteIndex индекс | LDeleteValue значение
LDeleteBefore индекс | LDeleteAfter индекс | LDeleteNode индекс
LPrint | LPrintBac | LPrintAddresses | LFind значение

SInitStack | SPush значение | SPop | STop | SIsEmptyStack | SPrint | SPrintAddresses
Q1InitQueue | Q1Insert значение | Q1Delete | Q1Front | Q1Back | Q1PrintQueue | Q1PrintAddresses

MInitArray вместимость | MAddEnd значение | MAddIndex значение индекс
MGet индекс | MDeleteIndex индекс | MReplace значение индекс
MLength | MPrint | MPrintAddresses | MResize | MDestroy

TInitTree | TAdd число | TDelete число | TFind число | TPrint | TPrintPryam | TPrintAddresses
HInitTable вместимость | HInsert ключ значение | HGet ключ | HContains ключ | HDelete ключ
HLength | HPrint | HPrintAddresses | HClear | HDestroy
CInitTable | CInsert ключ | CContains ключ | CDelete ключ | CPrint
`)
}
