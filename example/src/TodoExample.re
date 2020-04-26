open RestApi;

/** Define data model */
module TodoItem = {

  /** Define The model structure, it doesn't need to be a record type */
  type t = {
    userId: int,
    id: int,
    title: string,
    completed: bool
  };

  /** The namespace represents url path for the resource */
  let namespace = "todos";

  /** From external <json> data to internal representation <t> */
  let decode = json => Json.Decode.({
      userId: json |> field("userId", int),
      id: json |> field("id", int),
      title: json |> field("title", string),
      completed: json |> field("completed", bool)
  });

  /** The other way around, remember you don't need to serialize everythings. */
  let encode = (d:t) => Json.Encode.({
    [
      ("userId", int(d.userId)),
      ("id", int(d.id)),
      ("title", string(d.title)),
      ("completed", bool(d.completed))
    ]
    |> object_
  });

  /** You can also define an abritary function for your data manipulation */
  let string = (d:t) => d |> encode |> Json.stringify
}


module DummyEndpoint: RestApi.Endpoint = {
  
  let baseUrl = "https://jsonplaceholder.typicode.com"

  let urlWithPath = path => [|baseUrl, path|] |> Js.Array.joinWith("/")
}

module TodoApi = Make(DummyEndpoint, TodoItem)