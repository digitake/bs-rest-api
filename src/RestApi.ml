
(** The Header module type represent a module sig that envelop makeHeaders function. *)
module type Header =
    sig
        val makeHeaders: unit -> Fetch.HeadersInit.t
    end

(** This is a shape of finished module *)
module type API =
    sig
        type t  (** t is corresponding to API Data Model *)
        type unexpected = (int * string)
        type 'a promise = 'a Js.Promise.t

        type box = (t, unexpected) result promise
        type listBox = (t list, unexpected) result promise
        type emptyBox = (unit, unexpected) result promise

        type path = string
        
        val list: unit -> listBox
        val get: path -> box
        val post: t -> box
        val put: path -> t -> box
        val patch: path -> t -> box
        val delete: path -> emptyBox

        val fetch: path -> Fetch.RequestInit.t -> (Fetch.Response.t, unexpected) result promise
    end

module type DataModel =
    sig
        (** Abstract type t, normal OCaml convention. *)
        type t

        (** A string that represent the api path(think about it as a folder name of api) *)
        val namespace: string

        val decode: Js.Json.t -> t
        val encode: t -> Js.Json.t
    end

module type Endpoint =
    sig
        (** Base URL for the target Endpoint*)
        val baseUrl: string

        (** Build url path *)
        val urlWithPath: string -> string
    end

module MakeWithHeader(E:Endpoint)(D:DataModel)(H:Header):(API with type t = D.t) = 
    struct
        open PromiseMonad
        open Belt.Result
        include E
        include H

        type 'a promise = 'a Js.Promise.t
        type unexpected = (int * string)

        type t = D.t
        type path = string

        type box = (t, unexpected) result promise
        type listBox = (t list, unexpected) result promise
        type emptyBox = (unit, unexpected) result promise
        type rawBox = (Fetch.Response.t, unexpected) result promise

        let makeBody data = Fetch.BodyInit.make (data |> D.encode |> Js.Json.stringify)
            
        (** Low-level request with bare url, this will return Error if Fetch.Respose status is not ok *)
        let perform 
            ?method_ 
            ?(headers = makeHeaders())  (** Default to makeHeaders, which can be replaced *)
            ?body
            path: rawBox = 
            let _ = Js.log "test perform3" in
            let requestInit = Fetch.RequestInit.make ?method_ ~headers ?body () in
            let url = urlWithPath (D.namespace ^ path) in
            Fetch.fetchWithInit url requestInit
            >>- (fun resp ->
                match Fetch.Response.ok resp with (** Check if the response is ok *)
                | true -> Ok(resp)
                | false -> let open Fetch.Response in Error((status resp, statusText resp))
            )
        let decodeList = Json.Decode.list(D.decode)

        (** Take in result but return promose of result *)
        let extractResult decode box = 
            match box with
            | Ok(resp) -> Fetch.Response.json(resp)
                >>- (fun json -> Ok(json |> decode)) 
                >>/ (fun _ -> Error((-1, "Decode Error"))) 
            | Error(merr) -> return(Error(merr))

        let list () : listBox = (perform "/") >>= extractResult decodeList
        
        let get path = (perform path) >>= extractResult D.decode

        let post (data:t) = perform ~method_:Fetch.Post ~body:(makeBody data) "/" >>= extractResult D.decode

        let put path (data:t) = perform ~method_:Fetch.Put ~body:(makeBody data) path >>= extractResult D.decode

        let patch path (data:t) = perform ~method_:Fetch.Patch ~body:(makeBody data) path >>= extractResult D.decode

        let delete path = perform ~method_:Fetch.Delete path >>- (fun box -> map box ignore)

        let fetch path requestInit = 
            let url = urlWithPath (D.namespace ^ path) in
            Fetch.fetchWithInit url requestInit
            >>- (fun resp ->
                match Fetch.Response.ok resp with
                | true -> Ok(resp)
                | false -> let open Fetch.Response in Error((status resp, statusText resp))
            )
    end

module Make(E:Endpoint)(D:DataModel):(API with type t = D.t) =
    struct
        module HeaderDefault =
            struct
            let makeHeaders () = Fetch.HeadersInit.makeWithDict (Js.Dict.fromList [
                     ("Content-Type", "application/jsossn");
            ])
            end
        
        include MakeWithHeader(E)(D)(HeaderDefault)
    end
