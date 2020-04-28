open Jest
open Expect


module Item =
    struct
        type t = string

        let namespace = "test"

        let decode json = "decoded"
        let encode (d : t) = Json.Encode.string("encoded")

    end

module E =
struct
    let baseUrl = "https://jsonplaceholder.typicode.com"
    let urlWithPath path = [|baseUrl;path|] |>(Js.Array.joinWith (("/")[@reason.raw_literal "/"]))
end 

let () =

test "Functor should generate correct endpoint" (fun () ->
    let open ExpectJs in

    let module API = RestApi.Make(E)(Item) in

    
    1 |> expect |> toBe 1

)


