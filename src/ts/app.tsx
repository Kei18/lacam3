import * as React from "react";
import { createRoot } from "react-dom/client";
import { Helmet } from "react-helmet";

import "../scss/custom.scss";
import data from "../../data.yaml";
import images_gif from "../images/*.gif";
import images_png from "../images/*.png";

const App: React.SFC<{}> = () => {
  return (
    <div>
      <main role="main flex-shrink-0">
        <Helmet>
          <title>{data.title}</title>
          <meta charset="utf-8" />
          <meta property="og:type" content="article" />
          {data.description ? (
            <meta name="description" content={data.description} />
          ) : (
            ""
          )}
          <meta property="og:title" content={data.title} />
          {data.thumbnail && images_png[data.thumbnail] ? (
            <meta property="og:image" content={images_png[data.thumbnail]} />
          ) : (
            ""
          )}
          {data.site_url ? (
            <meta property="og:url" content={data.site_url} />
          ) : (
            ""
          )}
          {data.site_name ? (
            <meta property="og:site_name" content={data.site_name} />
          ) : (
            ""
          )}
          {data.description ? (
            <meta property="og:description" content={data.description} />
          ) : (
            ""
          )}
          <meta name="twitter:card" content="summary_large_image" />
          {data.twitter ? (
            <meta name="twitter:site" content={data.twitter} />
          ) : (
            ""
          )}
        </Helmet>

        <div className="container">
          {data.title ? <p className="h1 text-center">{data.title}</p> : ""}
          {data.authors ? (
            <div className="text-center row authors mt-3">
              {data.authors.map((item, i) => {
                if (item.url) {
                  return (
                    <div key={i} className="col-sm">
                      <a href={item.url}>{item.name}</a>
                    </div>
                  );
                } else {
                  return (
                    <div key={i} className="col-sm">
                      <span>{item.name}</span>
                    </div>
                  );
                }
              })}
            </div>
          ) : (
            ""
          )}

          {data.topLinks ? (
            <div className="text-center mt-3 top-links">
              {data.topLinks.map((item, i) => (
                <a key={i} href={item.url} className="badge bg-primary">
                  {item.name}
                </a>
              ))}
            </div>
          ) : (
            ""
          )}

          {data.headerImage ? (
            <div className="row mt-3">
              {images_gif[data.headerImage] ? (
                <img src={images_gif[data.headerImage]} className="img-fluid" />
              ) : (
                ""
              )}
              {images_png[data.headerImage] ? (
                <img src={images_png[data.headerImage]} className="img-fluid" />
              ) : (
                ""
              )}
            </div>
          ) : (
            ""
          )}

          {data.subLinks ? (
            <p className="text-center mt-3 sub-links">
              {data.subLinks.map((item, i) => (
                <a key={i} href={item.url} className="badge bg-success">
                  {item.name}
                </a>
              ))}
            </p>
          ) : (
            ""
          )}

          {data.abstract ? (
            <div className="mx-auto mt-5">
              <p className="h5">Overview</p>
              <div
                className="text-secondary"
                dangerouslySetInnerHTML={{ __html: data.abstract }}
              ></div>
            </div>
          ) : (
            ""
          )}

          {data.news ? (
            <div className="mx-auto mt-5">
              <p className="h5">News</p>
              <ul>
                {data.news.map((item, i) => (
                  <li key={i}>
                    {item.body}
                    {item.date ? " (" + item.date + ")" : ""}
                    {item.url ? (
                      <span>
                        {" "}
                        <a href={item.url}>[link]</a>
                      </span>
                    ) : (
                      ""
                    )}
                  </li>
                ))}
              </ul>
            </div>
          ) : (
            ""
          )}

          {data.video ? (
            <div className="mx-auto mt-5">
              <p className="h5">Video</p>
              <div className="text-center video-wrapper">
                <iframe
                  src={data.video}
                  width="854"
                  height="480"
                  frameborder="0"
                  allow="autoplay; fullscreen; picture-in-picture"
                  allowFullScreen
                ></iframe>
              </div>
            </div>
          ) : (
            ""
          )}

          {data.slides ? (
            <div className="mx-auto mt-5">
              <p className="h5">Slides</p>
              <div className="slide-wrapper">
                <iframe
                  src={data.slides}
                  allowFullScreen
                  scrolling="no"
                  allow="encrypted-media;"
                ></iframe>
              </div>
            </div>
          ) : (
            ""
          )}

          {data.citation ? (
            <div className="mx-auto mt-5">
              <p className="h5">Citation</p>
              <div className="citation mb-3 text-secondary">
                <pre>
                  <code>{data.citation}</code>
                </pre>
              </div>
            </div>
          ) : (
            ""
          )}

          {data.contact ? (
            <div className="mx-auto mt-5">
              <p className="h5">Contact</p>
              <p>{data.contact}</p>
            </div>
          ) : (
            ""
          )}
        </div>
      </main>

      {data.copyright ? (
        <footer className="footer mt-auto">
          <div className="container pt-3">
            <p className="text-center">
              &copy;{new Date().getFullYear()} {data.copyright}
            </p>
          </div>
        </footer>
      ) : (
        ""
      )}
    </div>
  );
};

const root = createRoot(document.querySelector("#root"));
root.render(<App />);
