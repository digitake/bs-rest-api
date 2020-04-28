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
TodoAPI.list()
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
        id: -1,  /** you can define id as option type if you don't like this -1 magic */
        userId: 1,
        title: "this is a test data 1234",
        completed: false
    }
    
    TodoAPIWithHeader.post(item)
    >>- (result => switch(result) {
        | Ok(item) => TodoItem.string(item)
        | Error((code, status)) => "<h2>"++string_of_int(code)++":"++status++"</h2>"
        }
        |> innerHTML(container)
    )
};

/** For custom path, it will fall back to Fetch.Request and Fetch.Response*/
let customRequest = Fetch.RequestInit.make(
    ~method_=Fetch.Post, 
    ~headers=Fetch.HeadersInit.make({ 
    "Content-Type": "application/json",
    }), 
    ~body=Fetch.BodyInit.make({|
    {
      "postId": 1,
      "name": "Test comment",
      "email": "Nikita@garfield.biz",
      "body": "hello from bs-rest-api"
    }
    |}),
    ()
);

TodoAPIWithHeader.fetch("posts/1/comments", customRequest)
    >>- (result => switch(result) {
        | Ok(rawResponse) => rawResponse->Fetch.Response.text>>-(d => innerHTML(container,d))
        | Error((code, status)) => "<h2>"++string_of_int(code)++":"++status++"</h2>"|> innerHTML(container)|>return
        }
    )

document->getElementById("todo-add")->setOnclick(addItem)