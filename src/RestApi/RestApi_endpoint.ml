open PromiseMonad

(** This is promise type, it represents a future value *)
type 'a promise = 'a Js.Promise.t

type headers = Fetch.Request.t
type body = Fetch.BodyInit.t
type expected = Fetch.response
type unexpected = (int * string)
(* type box = (expected, unexpected) result promise *)

