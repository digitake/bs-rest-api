
# bs-rest-api

REST in style, no more pain with the Functor force.

---
[![Package Quality](https://npm.packagequality.com/badge/@digitake%2Fbs-rest-api.png)](https://packagequality.com/#?package=@digitake%2Fbs-rest-api)
## Version: 1.3.2

## Dependencies
This package used `bs-json` for most of `encode/decode` part. It relied on `bs-fetch` as a communication layer. And also uses `bs-promise-monad` for syntatic suger, although it is not required.

---
## How to use?

#### 1. Define the module
```reasonml
module TodoItem = {
  /** 1.1 Define data model */
  type t = {
    userId: int,
    id: int,
    title: string,
    completed: bool
  };

  /** 1.2 The namespace represents url path for the resource */
  let namespace = "todos";

  /** 1.3 Create a decoder */
  let decode = json => Json.Decode.({
      userId: json |> field("userId", int),
      id: json |> field("id", int),
      title: json |> field("title", string),
      completed: json |> field("completed", bool)
  });

  /** 1.4 Create an encoder */
  let encode = (d:t) => Json.Encode.({
    [
      ("userId", int(d.userId)),
      ("id", int(d.id)),
      ("title", string(d.title)),
      ("completed", bool(d.completed))
    ]
    |> object_
  });
}
```

#### 2. Define the Endpoint
```reasonml
module DummyEndpoint: RestApi.Endpoint = {
  /** 2.1 Define base url */
  let baseUrl = "https://jsonplaceholder.typicode.com"

  /** 2.2 Define a urlWithPath function */
  let urlWithPath = path => [|baseUrl, path|] |> Js.Array.joinWith("/")
}
```

#### 3. Build the API
```reasonml
module TodoAPI = RestApi.Make(DummyEndpoint, TodoItem)
```

#### 4. Use the API
```reasonml
open PromiseMonad;
TodoAPI.list()
>>- (result => switch(result) {
    | Ok(items) => Js.log(items)
    | Error((code, status)) => Js.log2(code, status)
    }
);

TodoAPI.get("1")
>>- (result => switch(result) {
    | Ok(item) => Js.log(item)
    | Error((code, status)) => Js.log2(code, status)
    }
);
    
TodoAPI.post({
        id: -1,
        userId: 1,
        title: "Easy API with free type power",
        completed: false
    }) >>- (result => switch(result) {
    | Ok(item) => Js.log(item)
    | Error((code, status)) => Js.log2(code, status)
    }
);

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
        | Ok(rawResponse) => rawResponse->Fetch.Response.text|> innerHTML(container)
        | Error((code, status)) => "<h2>"++string_of_int(code)++":"++status++"</h2>"|> innerHTML(container)
        }
    )
```

#### Extra - Use it with custom request header suah as Token?
```reasonml
module TokenHeader = {
  let makeHeaders () => HeadersInit.make({ 
      "Content-Type": "application/json",
      "Authorization": "token-this-here you can pull it with your custom logic."
    })
}

module TodoAPIWithHeader = MakeWithHeader(DummyEndpoint, TodoItem, TokenHeader)

TodoAPIWithHeader.get("1")
>>- (result => switch(result) {
    | Ok(item) => Js.log(item)
    | Error((code, status)) => Js.log2(code, status)
    }
);
```

### Changes

#### 1.3.2

- Patch security on dependencies
- Fix a security issue from PR #1

#### 1.3.0
- Add MakeWithHeader
- Rename module name to be more consistent
- Remove unused module.

#### 1.2.0
- Publish to npm
- Add detail example
- API Can be used now.

#### 1.0.0

- Initial commit with compilable code

### TODO
- Implement npm publish action
- Improve Module Type Interface(Try to reduce types exposure)
- Implement interactive demo to pull API
- Implement API DataModel Generator(start with django-rest-framework)

PRs are welcome. Comments or Requests are also welcome. Thank you.
