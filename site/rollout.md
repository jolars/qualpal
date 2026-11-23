# Site rollout

Run the Site workflow and inspect the verified artifact in a local Wrangler
preview. Record the active `qualpal` Worker deployment and any `qualpal.cc/docs*`
route in Cloudflare before the first deployment.

Deploy the verified artifact to the existing `qualpal` Worker. If the old
`qualpal-docs` route exists, remove that route after the apex deployment, then
rerun the workflow's public smoke checks. Roll back the apex deployment and
restore the recorded route if those checks fail.

After the public checks pass, dispatch the Site workflow on `main` with
`publish_legacy_pages` enabled. It builds the pre-migration Doxygen tree and
publishes compatibility pages while retaining the public assets. The redirect
pages carry the original query and fragment to the compatibility URLs.
