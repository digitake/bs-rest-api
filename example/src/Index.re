open TodoExample;
open PromiseMonad;

TodoApi.list()
>>- (x=>Js.log(x))