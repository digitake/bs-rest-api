open TodoExample;
open PromiseMonad;


type element;
type document;
[@bs.val] external document : document = "document";
[@bs.send] external getElementById : (document, string) => element = "getElementById";
[@bs.set] external innerHTML : (element, 'a) => unit = "innerHTML";
[@bs.set] external setOnclick : (element, 'a) => unit = "onclick";

let container = document->getElementById("todos-container");

/** This is how to use the api */
TodoApi.list()
>>- (result => switch(result) {
    | Ok(items) =>{
        items 
        |> List.map((item:TodoItem.t) => "<li>"++ TodoItem.string(item) ++ "</li>") 
        |> Array.of_list
        |> Js.Array.joinWith("\n")
        |> (s) => "<ul>" ++ s ++ "</ul>"
    }
    | Error((code, status)) => "<h2>"++string_of_int(code)++":"++status++"</h2>"
    }
    |> innerHTML(container)
)

/** This is how to post */
let addItem = () => {
    let item:TodoItem.t={
        id: -1,
        userId: 1,
        title: "this is a test data 1234",
        completed: false
    }
    TodoApi.post(item)
}

document->getElementById("todo-add")->setOnclick(addItem)