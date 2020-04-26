module type Data =
    sig
        (** Abstract type t, normal OCaml convention. *)
        type t

        (** A string that represent the api path(think about it as a folder name of api) *)
        val namespace: string

        val decode: Js.Json.t -> t
        val encode: t -> Js.Json.t
    end