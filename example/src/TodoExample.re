open RestApi;

/** Define data model */
module ItemModel : DataModel = {

  type t = {
    userId: int,
    id: int,
    title: string,
    completed: bool
  };

  let namespace = "todos";

  let decode = json => Json.Decode.({
      userId: json |> field("userId", int),
      id: json |> field("id", int),
      title: json |> field("title", string),
      completed: json |> field("completed", bool)
  });

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


module Endpoint = {

}

module TodoApi = Make(Endpoint, ItemModel)